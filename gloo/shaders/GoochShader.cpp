#include "GoochShader.hpp"

namespace GLOO {

GoochShader::GoochShader() : PhongShader({
  {GL_VERTEX_SHADER, "gooch.vert"},
  {GL_FRAGMENT_SHADER, "gooch.frag"}
}) {}

}  // namespace GLOO
