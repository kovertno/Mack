#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;

layout (std140, binding = 0) uniform Matrices {
	mat4 projection;
	mat4 view;
};

uniform mat4 model;
uniform float scale;

void main() {
	gl_Position = projection * view * model * vec4(aPos + (aNorm * scale), 1.0);
}