#version 300 es
precision mediump float;

uniform sampler2D u_TextureUnit;
in vec4 v_pos;
out vec4 my_color;


void main() {
    my_color = texture2D(u_TextureUnit, vec2((v_pos.x + 1.0) * 0.5, (1.0-v_pos.y) * 0.5));
}
