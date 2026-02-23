#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform DirLight dirLight;

uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

vec3 CalcDirLight(DirLight dirLight, Material material, vec3 FragPos, vec3 normal, vec3 viewDir);

void main() {
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 normal = normalize(Normal);

	vec3 result = CalcDirLight(dirLight, material, FragPos, normal, viewDir);
	
	FragColor = vec4(result, 1.0); 
}

vec3 CalcDirLight(DirLight dirLight, Material material, vec3 FragPos, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-dirLight.direction);

	float diff = max(dot(lightDir, normal), 0.0f);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	vec3 ambient = dirLight.ambient * material.ambient;
	vec3 diffuse = dirLight.diffuse * diff * material.diffuse;
	vec3 specular = dirLight.specular * spec * material.specular;

	return (ambient + diffuse + specular);
}