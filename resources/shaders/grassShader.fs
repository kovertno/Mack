#version 330 core

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

uniform vec3 viewPos;
uniform bool useSpotLight;

in vec3 Normal;
in vec3 FragPos;

vec3 CalcDirLight(vec3 color, vec3 viewDir, vec3 normal);
vec3 CalcSpotLight(vec3 color, vec3 viewDir, vec3 normal);

void main() {
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 normal = normalize(Normal);
	vec3 color = vec3(0.0f, 0.5f, 0.1f);

	vec3 result = CalcDirLight(color, viewDir, normal);
	if(useSpotLight) 
		result += CalcSpotLight(color, viewDir, normal);

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(vec3 color, vec3 viewDir, vec3 normal) {
	vec3 lightDir = normalize(-dirLight.direction);

	float diff = max(abs(dot(lightDir, normal)), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 16.0);

	vec3 ambient = color * dirLight.ambient;
	vec3 diffuse = color * diff * dirLight.diffuse;
	vec3 specular = color * spec * dirLight.specular;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(vec3 color, vec3 viewDir, vec3 normal) {
	vec3 lightDir = normalize(spotLight.position - FragPos);

	float diff = max(abs(dot(lightDir, normal)), 0.0);

	float theta = dot(lightDir, normalize(-spotLight.direction));
	float epsilon = spotLight.cutOff - spotLight.outCutOff;

	float intensity = clamp(((theta - spotLight.outCutOff) / epsilon), 0.0, 1.0);

	float spec = pow(theta, 16.0);

	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);

	ambient += spotLight.ambient * color;
	diffuse += spotLight.diffuse * diff * color;
	specular += spotLight.specular * spec * color;

	ambient *= intensity;
	diffuse *= intensity;
	specular *= intensity;

	return (ambient + diffuse + specular);
}