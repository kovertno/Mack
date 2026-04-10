#version 420 core

in VS_OUT {
	vec3 TexCoords;
} fs_in;

out vec4 FragColor;

uniform samplerCube skybox;

void main() {
	FragColor = texture(skybox, fs_in.TexCoords);
}