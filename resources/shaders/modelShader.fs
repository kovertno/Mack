#version 400 core

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPos;

out vec4 FragColor;

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

struct SpotLight {
	vec3 position;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float cutOff;
	float outCutOff;
};
uniform SpotLight spotLight;

#define MAX_DIFFUSE 4
#define MAX_SPECULAR 4
struct Material {
	sampler2D texture_diffuse[MAX_DIFFUSE];
	sampler2D texture_specular[MAX_SPECULAR];
	vec3 baseColor;

	float shininess;
};
uniform Material material;

uniform int diffNum;
uniform int specNum;
uniform vec3 viewPos;

uniform bool useSpotLight;

vec3 CalcDirLight(vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(vec3 normal, vec3 viewDir);

void main() {
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = CalcDirLight(normal, viewDir);
	if (useSpotLight) {
		result += CalcSpotLight(normal, viewDir);
	}

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-dirLight.direction);
	float diff = max(dot(lightDir, normal), 0.0);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);

	if(diffNum > 0) {
		for (int i = 0; i < diffNum; ++i) {
			ambient += dirLight.ambient * vec3(texture(material.texture_diffuse[i], TexCoords));
			diffuse += dirLight.diffuse * diff * vec3(texture(material.texture_diffuse[i], TexCoords));
		}
	}
	else {
		ambient += dirLight.ambient * material.baseColor;
		diffuse += dirLight.diffuse * diff * material.baseColor;
	}

	vec3 specular = vec3(0.0);
	for (int i = 0; i < specNum; ++i) {
		specular += dirLight.specular * spec * vec3(texture(material.texture_specular[i], TexCoords));
	}

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(spotLight.position - FragPos);
	float diff = max(dot(lightDir, normal), 0.0);

	float theta = dot(lightDir, normalize(-spotLight.direction));
	float epsilon = spotLight.cutOff - spotLight.outCutOff;

	float intensity = clamp((theta - spotLight.outCutOff) / epsilon, 0.0, 1.0);

	float spec = pow(theta, material.shininess);

	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.05);
	vec3 specular = vec3(0.0);

	if(diffNum > 0) {
		for (int i = 0; i < diffNum; ++i) {
			ambient += spotLight.ambient * vec3(texture(material.texture_diffuse[i], TexCoords));
			diffuse += spotLight.diffuse * diff * vec3(texture(material.texture_diffuse[i], TexCoords)); 
		}
	}
	else {
		ambient += spotLight.ambient * material.baseColor;
		diffuse += spotLight.diffuse * diff * material.baseColor;
	}

	for (int i = 0; i < specNum; ++i) {
		specular += spotLight.specular * spec * vec3(texture(material.texture_specular[i], TexCoords));
	}

	ambient *= intensity;
	diffuse *= intensity;
	specular *= intensity;

	return (ambient + diffuse + specular);
}