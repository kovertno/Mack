#version 400 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

out vec4 FragColor;

#define MAX_DIFFUSE 4
uniform sampler2D texture_diffuse[MAX_DIFFUSE];

#define MAX_SPECULAR 4
uniform sampler2D texture_specular[MAX_SPECULAR];

uniform int diffNum;
uniform int specNum;


void main() {

	vec3 diffuse = vec3(0.0);
	for (int i = 0; i < diffNum; i++) {
		diffuse += vec3(texture(texture_diffuse[i], TexCoords));
	}

	vec3 specular = vec3(0.0);
	for (int i = 0; i < specNum; i++) {
		specular += vec3(texture(texture_specular[i], TexCoords));  
	}
	
	FragColor = vec4(diffuse + specular, 1.0);
	//FragColor = vec4(TexCoords, 0.0, 1.0);
}