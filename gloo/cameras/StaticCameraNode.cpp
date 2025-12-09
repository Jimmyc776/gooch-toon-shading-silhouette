#include "StaticCameraNode.hpp"
#include "gloo/components/CameraComponent.hpp"

namespace GLOO {

StaticCameraNode::StaticCameraNode(float fov, float aspect, glm::vec3 position)
    : SceneNode() {
  auto camera = make_unique<CameraComponent>(fov, aspect, 0.1f, 100.0f);
  AddComponent(std::move(camera));
  GetTransform().SetPosition(position);
}

}  // namespace GLOO
