#ifndef GLOO_OUTLINE_SHADER_H_
#define GLOO_OUTLINE_SHADER_H_

#include "PhongShader.hpp"

namespace GLOO {

class OutlineShader : public PhongShader {
 public:
  OutlineShader() : PhongShader({
      {GL_VERTEX_SHADER, "phong.vert"},
      {GL_FRAGMENT_SHADER, "outline.frag"}
  }) {}
};

}  // namespace GLOO

#endif
