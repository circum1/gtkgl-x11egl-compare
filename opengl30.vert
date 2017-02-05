#version 130

in vec3 position;
uniform mat4 mvp;
smooth out vec4 v_pos;

void main() {
  gl_Position = mvp * vec4(position, 1.0);
  v_pos = vec4(position, 1.0);
}
