#ifndef SPLINE_VIEWER_APP_H_
#define SPLINE_VIEWER_APP_H_

#include "gloo/Application.hpp"
#include "gloo/parsers/ObjParser.hpp"

namespace GLOO {
class OrbitCameraNode;

enum class ShadingMode { Phong, Gooch, Toon };

class SplineViewerApp : public Application {
  public:
    SplineViewerApp(const std::string& app_name, glm::ivec2 window_size);
    void SetupScene() override;
    void Tick(double delta_time, double current_time);
    void ProcessInput() override;

  private:
    float elapsed_time_ = 0.0f;
    SceneNode* mesh_node_ = nullptr;
    SceneNode* outline_node_ = nullptr;
    ObjParser::ParsedData parsed_data;
    std::string model_name = "bunny";  // "teapot", "bunny", "quad"
    ShadingMode mode_ = ShadingMode::Phong;     // Phong, Gooch, Toon
    bool outlines_enabled_ = false;
    std::unordered_map<int, bool> key_was_down_;
};
}  // namespace GLOO

#endif
