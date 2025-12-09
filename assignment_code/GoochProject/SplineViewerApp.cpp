#include "SplineViewerApp.hpp"

#include <fstream>

#include "gloo/cameras/StaticCameraNode.hpp"
#include "gloo/lights/AmbientLight.hpp"
#include "gloo/lights/PointLight.hpp"
#include "gloo/components/LightComponent.hpp"
#include "gloo/components/RenderingComponent.hpp"
#include "gloo/debug/PrimitiveFactory.hpp"  // for CreateQuad
#include "gloo/shaders/PhongShader.hpp"
#include "gloo/components/ShadingComponent.hpp"
#include "gloo/components/MaterialComponent.hpp"
#include "gloo/Material.hpp"
#include "gloo/parsers/ObjParser.hpp"
#include <glm/glm.hpp>
#include <unordered_map>

#include "gloo/shaders/GoochShader.hpp"
#include "gloo/shaders/OutlineShader.hpp"
#include "gloo/shaders/ToonShader.hpp"

namespace GLOO {

SplineViewerApp::SplineViewerApp(const std::string& app_name,
                                 glm::ivec2 window_size)
    : Application(app_name, window_size) {
}

std::unique_ptr<NormalArray> GenerateNormals(const PositionArray& positions,
                                               const IndexArray& indices) {
  auto normals = make_unique<NormalArray>(positions.size(), glm::vec3(0.0f));

  // Calculate face normals and accumulate
  for (size_t i = 0; i < indices.size(); i += 3) {
    unsigned int i0 = indices[i];
    unsigned int i1 = indices[i + 1];
    unsigned int i2 = indices[i + 2];

    glm::vec3 v0 = positions[i0];
    glm::vec3 v1 = positions[i1];
    glm::vec3 v2 = positions[i2];

    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 face_normal = glm::normalize(glm::cross(edge1, edge2));

    // Accumulate to vertex normals
    (*normals)[i0] += face_normal;
    (*normals)[i1] += face_normal;
    (*normals)[i2] += face_normal;
  }

  // Normalize all vertex normals
  for (auto& n : *normals) {
    n = glm::normalize(n);
  }

  return normals;
}

void SplineViewerApp::SetupScene() {
  SceneNode& root = scene_->GetRootNode();

  // === CAMERA ===
  auto camera_node = make_unique<StaticCameraNode>(
      45.0f, 1.0f,   // FOV, aspect (1.0 for now)
      glm::vec3(0.0f, 3.0f, 10.0f)); // up vector

  scene_->ActivateCamera(camera_node->GetComponentPtr<CameraComponent>());
  root.AddChild(std::move(camera_node));

  // === LIGHTS ===
  auto ambient_light = std::make_shared<AmbientLight>();
  ambient_light->SetAmbientColor(glm::vec3(0.1f));
  root.CreateComponent<LightComponent>(ambient_light);

  auto point_light = std::make_shared<PointLight>();
  point_light->SetDiffuseColor(glm::vec3(2.0f));
  point_light->SetSpecularColor(glm::vec3(0.7f, 0.7f, 0.7f));  // softer specular
  point_light->SetAttenuation(glm::vec3(1.0f, 0.1f, 0.02f));

  auto point_light_node = make_unique<SceneNode>();
  point_light_node->CreateComponent<LightComponent>(point_light);
  point_light_node->GetTransform().SetPosition(glm::vec3(3.0f, 3.0f, 2.0f));
  root.AddChild(std::move(point_light_node));

  // === LOAD MESH ===
  auto mesh_node = make_unique<SceneNode>();

  // Load object
  bool success;
  if (model_name == "quad") {
    // special case for quad
    auto vertex_obj = PrimitiveFactory::CreateQuad();
    mesh_node->CreateComponent<RenderingComponent>(std::move(vertex_obj));
    mesh_node->GetTransform().SetScale(glm::vec3(2.0f));
    mesh_node_ = mesh_node.get();
    root.AddChild(std::move(mesh_node));
    return;
  } else if (model_name == "bunny") {
    parsed_data = ObjParser::Parse("../assignment_code/GoochProject/models/bunny_1k.obj", success);
  } else if (model_name == "teapot") {
    parsed_data = ObjParser::Parse("../assignment_code/GoochProject/models/teapot.obj", success);
  } else {
    std::cerr << "Unknown model name: " << model_name << ". Defaulting to teapot." << std::endl;
    model_name = "teapot";
    parsed_data = ObjParser::Parse("../assignment_code/GoochProject/models/teapot.obj", success);
  }

  if (!success) {
    std::cerr << "Failed to load obj!" << std::endl;
    return;
  }

  std::cout << "Loaded successfully!" << std::endl;
  std::cout << "Positions: " << (parsed_data.positions ? parsed_data.positions->size() : 0) << std::endl;
  std::cout << "Normals: " << (parsed_data.normals ? parsed_data.normals->size() : 0) << std::endl;
  std::cout << "Indices: " << (parsed_data.indices ? parsed_data.indices->size() : 0) << std::endl;

  // Generate normals if missing
  if (!parsed_data.normals || parsed_data.normals->empty()) {
    std::cout << "Generating normals..." << std::endl;
    parsed_data.normals = GenerateNormals(*parsed_data.positions, *parsed_data.indices);
  }

  // === OUTLINE ===
  if (outlines_enabled_) {
    std::cout << "Creating outline node..." << std::endl;

    auto outline_node = make_unique<SceneNode>();

    auto outline_vertex_obj = make_unique<VertexObject>();
    outline_vertex_obj->UpdatePositions(GLOO::make_unique<PositionArray>(*parsed_data.positions));
    outline_vertex_obj->UpdateNormals(GLOO::make_unique<NormalArray>(*parsed_data.normals));
    outline_vertex_obj->UpdateIndices(GLOO::make_unique<IndexArray>(*parsed_data.indices));

    std::cout << "Outline vertex  " << parsed_data.positions->size()
            << " pos, " << parsed_data.indices->size() << " idx" << std::endl;

    outline_node->CreateComponent<RenderingComponent>(std::move(outline_vertex_obj));

    auto outline_shader = std::make_shared<OutlineShader>();
    outline_node->CreateComponent<ShadingComponent>(outline_shader);

    auto black_material = std::make_shared<Material>();
    black_material->SetDiffuseColor(glm::vec3(0.0f, 0.0f, 0.0f));
    black_material->SetSpecularColor(glm::vec3(0.0f));
    outline_node->CreateComponent<MaterialComponent>(black_material);

    outline_node->GetTransform().SetPosition(glm::vec3(0.0f));

    outline_node_ = outline_node.get();  // Store pointer for animation
    std::cout << "Outline node added to scene!" << std::endl;
    root.AddChild(std::move(outline_node));
  }

  // === CREATE MESH ===
  auto positions = std::move(parsed_data.positions);
  auto normals = std::move(parsed_data.normals);
  auto indices = std::move(parsed_data.indices);

  auto vertex_obj = make_unique<VertexObject>();
  vertex_obj->UpdatePositions(std::move(positions));
  vertex_obj->UpdateNormals(std::move(normals));
  vertex_obj->UpdateIndices(std::move(indices));

  mesh_node->CreateComponent<RenderingComponent>(std::move(vertex_obj));

  if (model_name == "quad") {
    mesh_node->GetTransform().SetScale(glm::vec3(2.0f));
  } else if (model_name == "bunny") {
    mesh_node->GetTransform().SetScale(glm::vec3(7.0f));
  } else {
    mesh_node->GetTransform().SetScale(glm::vec3(0.7f));
  }

  if (outline_node_) {
    outline_node_->GetTransform().SetScale(mesh_node->GetTransform().GetScale() * 0.96f);
  }

  std::shared_ptr<ShaderProgram> shader;
  if (mode_ == ShadingMode::Phong) {
    shader = std::make_shared<PhongShader>();
    std::cout << "Using Phong shading." << std::endl;
  } else if (mode_ == ShadingMode::Gooch) {
    shader = std::make_shared<GoochShader>();
    std::cout << "Using Gooch shading." << std::endl;
  } else if (mode_ == ShadingMode::Toon) {
    shader = std::make_shared<ToonShader>();
    std::cout << "Using Toon shading." << std::endl;
  } else {
    shader = std::make_shared<PhongShader>();
    std::cout << "Unknown shading mode, defaulting to Phong." << std::endl;
  }
  mesh_node->CreateComponent<ShadingComponent>(shader);

  mesh_node->GetTransform().SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
  mesh_node_ = mesh_node.get();  // Store pointer for animation

  auto material_ptr = std::make_shared<Material>();
  material_ptr->SetDiffuseColor(glm::vec3(0.7f, 0.3f, 0.2f));   // neutral gray
  material_ptr->SetSpecularColor(glm::vec3(0.2f, 0.2f, 0.2f));  // weaker specular
  material_ptr->SetShininess(8.0f);                             // less shiny
  auto material_comp = mesh_node->CreateComponent<MaterialComponent>(material_ptr);

  root.AddChild(std::move(mesh_node));
}

void SplineViewerApp::Tick(double delta_time, double current_time) {
  // Call base implementation to update scene, render, etc.
  Application::Tick(delta_time, current_time);

  elapsed_time_ += static_cast<float>(delta_time);

  if (mesh_node_) {
    // Rotate main mesh
    auto& transform = mesh_node_->GetTransform();
    glm::quat rot = transform.GetRotation();
    float angle_radians = delta_time * 1.0f;  // adjust speed if needed
    glm::quat delta_rotation = glm::angleAxis(angle_radians, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat new_rot = delta_rotation * rot;
    transform.SetRotation(new_rot);

    if (outline_node_) {
      outline_node_->GetTransform().SetRotation(new_rot);
    }
  }

  // For fixed position (uncomment if needed)
  // if (mesh_node_) {
  //   glm::quat fixed_rot = glm::angleAxis(glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  //   mesh_node_->GetTransform().SetRotation(fixed_rot);
  //   if (outline_node_) {
  //     outline_node_->GetTransform().SetRotation(fixed_rot);
  //   }
  // }
}

void SplineViewerApp::ProcessInput() {
  GLFWwindow* window = GetWindowHandle();
  bool need_reset = false;

  auto was_down = [this](int key) {
    auto it = key_was_down_.find(key);
    return it != key_was_down_.end() && it->second;
  };

  auto update_key = [this](int key, int state) {
    key_was_down_[key] = (state == GLFW_PRESS);
  };

  // Helper: handle one key with edge detection (press event only)
  std::function<void(int, const std::function<void()>&)> handle_key =
      [&](int key, const std::function<void()>& on_press) {
        int state = glfwGetKey(window, key);
        bool down_now = (state == GLFW_PRESS);
        bool down_before = was_down(key);

        if (down_now && !down_before) {  // rising edge
          on_press();
        }
        update_key(key, state);
      };

  // 1: Phong
  handle_key(GLFW_KEY_1, [&]() {
    mode_ = ShadingMode::Phong;
    need_reset = true;
  });

  // 2: Gooch
  handle_key(GLFW_KEY_2, [&]() {
    mode_ = ShadingMode::Gooch;
    need_reset = true;
  });

  // 3: Toon
  handle_key(GLFW_KEY_3, [&]() {
    mode_ = ShadingMode::Toon;
    need_reset = true;
  });

  // O: toggle outlines
  handle_key(GLFW_KEY_O, [&]() {
    outlines_enabled_ = !outlines_enabled_;
    need_reset = true;
  });

  // M: switch model
  handle_key(GLFW_KEY_M, [&]() {
    if (model_name == "teapot")
      model_name = "bunny";
    else
      model_name = "teapot";
    need_reset = true;
  });

  if (need_reset) {
    auto root = GLOO::make_unique<SceneNode>();
    scene_ = GLOO::make_unique<Scene>(std::move(root));
    SetupScene();
  }
}

}  // namespace GLOO
