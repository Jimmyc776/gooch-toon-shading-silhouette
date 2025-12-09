#ifndef GLOO_STATIC_CAMERA_NODE_H_
#define GLOO_STATIC_CAMERA_NODE_H_

#include "gloo/SceneNode.hpp"
#include "gloo/components/CameraComponent.hpp"

namespace GLOO {
class StaticCameraNode : public SceneNode {
 public:
  StaticCameraNode(float fov, float aspect, glm::vec3 position);
};
}  // namespace GLOO

#endif
