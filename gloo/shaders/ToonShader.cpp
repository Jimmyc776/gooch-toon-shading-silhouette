#include "ToonShader.hpp"

namespace GLOO {

ToonShader::ToonShader()
    : PhongShader({
          {GL_VERTEX_SHADER, "gooch.vert"},   // reuse Gooch vertex shader
          {GL_FRAGMENT_SHADER, "toon.frag"}   // new toon fragment shader
      }) {
}

}  // namespace GLOO
