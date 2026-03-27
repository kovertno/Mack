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

out vec4 FragColor;

const float near = 0.1;
const float far = 100.0;

vec3 CalcDirLight(vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(vec3 normal, vec3 viewDir);
float LinearizeDepth(float depth);

void main() {
	vec3 viewDir = normalize(viewPos - FragPos);

	vec3 normal = normalize(Normal);

	vec3 result = CalcDirLight(normal, viewDir);
	if(useSpotLight)
		result += CalcSpotLight(normal, viewDir);
	
	//float depth = LinearizeDepth(gl_FragCoord.z) / far;
	FragColor = vec4(result, 1.0); 
	//FragColor = vec4(vec3(depth), 1.0);
}

vec3 CalcDirLight(vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(-dirLight.direction);

	float diff = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	vec3 ambient = dirLight.ambient * material.ambient;
	vec3 diffuse = dirLight.diffuse * diff * material.diffuse;
	vec3 specular = dirLight.specular * spec * material.specular;

	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(spotLight.position - FragPos);

	float diff = max(dot(lightDir, normal), 0.0);

	float theta = dot(lightDir, normalize(-spotLight.direction));
	float epsilon = spotLight.cutOff - spotLight.outCutOff;

	float intensity = clamp(((theta - spotLight.outCutOff) / epsilon), 0.0, 1.0);

	float spec = pow(theta, material.shininess);

	vec3 ambient = vec3(0.0);
	vec3 diffuse = vec3(0.0);
	vec3 specular = vec3(0.0);

	ambient += spotLight.ambient * material.ambient;
	diffuse += spotLight.diffuse * diff * material.diffuse;
	specular += spotLight.specular * spec * material.specular;

	ambient *= intensity;
	diffuse *= intensity;
	specular *= intensity;

	return (ambient + diffuse + specular);
}

float LinearizeDepth(float depth) {
	float ndc = depth * 2.0 - 1.0; 
	return (2.0 * near * far) / (far + near - ndc * (far - near));
}