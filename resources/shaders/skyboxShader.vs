#version 330 core

layout (location = 0) in vec3 aPos;

out VS_OUT {
	vec3 TexCoords;
} vs_out;

uniform mat4 projection;
uniform mat4 view;

void main() {
	vec4 position = projection * view * vec4(aPos, 1.0);
	gl_Position = position.xyww;

	vs_out.TexCoords = aPos;
}