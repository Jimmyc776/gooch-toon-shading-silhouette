#ifndef GLOO_TOON_SHADER_H_
#define GLOO_TOON_SHADER_H_

#include "PhongShader.hpp"
#include <unordered_map>
#include <string>

namespace GLOO {

class ToonShader : public PhongShader {
 public:
  ToonShader();

  // Optional: helper methods to set Toon-specific uniforms
  void SetCoolColor(const glm::vec3& color) {
    SetUniform("cool_color", color);
  }

  void SetWarmColor(const glm::vec3& color) {
    SetUniform("warm_color", color);
  }

  void SetAlpha(float alpha) {
    SetUniform("alpha", alpha);
  }

  void SetBeta(float beta) {
    SetUniform("beta", beta);
  }
};

}  // namespace GLOO

#endif
