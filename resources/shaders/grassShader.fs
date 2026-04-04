#version 330 core

in VS_OUT {
	vec3 Normal;
	vec3 FragPos;
} fs_in;

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

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
#define NUM_POINT_LIGHTS 5
uniform PointLight pointLight[NUM_POINT_LIGHTS];

uniform vec3 viewPos;
uniform bool useSpotLight;
uniform bool usePointLight;

vec3 CalcDirLight(vec3 color, vec3 viewDir, vec3 normal);
vec3 CalcSpotLight(vec3 color, vec3 viewDir, vec3 normal);
vec3 CalcPointLight(PointLight pointLight, vec3 color, vec3 viewDir, vec3 normal);

void main() {
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 normal = normalize(fs_in.Normal);
	vec3 color = vec3(0.0f, 0.5f, 0.1f);

	vec3 result = CalcDirLight(color, viewDir, normal);
	if(useSpotLight) 
		result += CalcSpotLight(color, viewDir, normal);

	if (usePointLight) {
		for (int i = 0; i < NUM_POINT_LIGHTS; ++i) {
			result += CalcPointLight(pointLight[i], color, normal, viewDir);
		}
	}

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
	vec3 lightDir = normalize(spotLight.position - fs_in.FragPos);

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

vec3 CalcPointLight(PointLight pointLight, vec3 color, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(pointLight.position - fs_in.FragPos);

	float diff = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), 16.0);

	float distance = length(pointLight.position - fs_in.FragPos);
	float attenuation = 1.0 / (pointLight.constant + (pointLight.linear * distance) + (pointLight.quadratic * (distance * distance)));

	vec3 ambient = pointLight.ambient * color;
	vec3 diffuse = pointLight.diffuse * diff * color;
	vec3 specular = pointLight.specular * spec * color;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}