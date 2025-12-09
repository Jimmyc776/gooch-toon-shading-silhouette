#version 330 core
in vec3 world_position;
in vec3 world_normal;

out vec4 frag_color;

void main() {
  frag_color = vec4(0.0, 0.0, 0.0, 1.0);  // Pure black
}
