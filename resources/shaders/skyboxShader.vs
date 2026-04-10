#version 420 core

layout (location = 0) in vec3 aPos;

layout (std140) uniform Matrices {
	mat4 projection;
	mat4 view;
};

out VS_OUT {
	vec3 TexCoords;
} vs_out;

void main() {
	vec4 position = projection * mat4(mat3(view)) * vec4(aPos, 1.0);
	gl_Position = position.xyww;

	vs_out.TexCoords = aPos;
}