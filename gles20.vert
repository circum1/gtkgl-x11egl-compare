#version 300 es
uniform mat4 mvp;
in vec3 position;
out vec4 v_pos;

void main() {
  gl_Position = mvp * vec4(position, 1.0);
  v_pos = vec4(position, 1.0);
}
