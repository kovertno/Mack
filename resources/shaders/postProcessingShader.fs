#version 330 core

in VS_OUT {
	vec2 TexCoords;
} fs_in;

out vec4 FragColor;

uniform sampler2D screenTexture;

void main() {
	FragColor = texture(screenTexture, fs_in.TexCoords);
}