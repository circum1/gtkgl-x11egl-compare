#version 130

uniform sampler2D u_TextureUnit;
smooth in vec4 v_pos;
out vec4 outputColor;

void main() {
//    outputColor = texture2D(u_TextureUnit, vec2((v_pos.x + 1.0) * 0.5, (1.0-v_pos.y) * 0.5));
    outputColor = vec4(0.5, 0, 0, 1);
}
