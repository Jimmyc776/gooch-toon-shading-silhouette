#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model_matrix;
uniform mat3 normal_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

out vec3 world_position;
out vec3 world_normal;

void main() {
  world_position = (model_matrix * vec4(position, 1.0)).xyz;
  world_normal = normal_matrix * normal;
  vec3 extruded_pos = world_position - world_normal * 0.04;
  gl_Position = projection_matrix * view_matrix * vec4(extruded_pos, 1.0);
  // gl_Position = projection_matrix * view_matrix * vec4(world_position, 1.0);
}
