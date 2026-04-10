#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

layout (std140, binding = 0) uniform Matrices {
	mat4 projection;
	mat4 view;
};

out VS_OUT {
	vec3 Normal;
	vec3 FragPos;
} vs_out;

uniform mat4 model;

void main() {
	vs_out.Normal = mat3(transpose(inverse(model))) * aNorm;

	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}