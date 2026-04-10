#version 420 core

layout (std140, binding = 1) uniform Data {
	vec3 viewPos;
};

in VS_OUT {
	vec3 Normal;
	vec3 FragPos;
} fs_in;

out vec4 FragColor;

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

uniform bool useSpotLight;
uniform bool usePointLight;

const float near = 0.1;
const float far = 100.0;

vec3 CalcDirLight(vec3 normal, vec3 viewDir);
vec3 CalcSpotLight(vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 viewDir);

float LinearizeDepth(float depth);

void main() {
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	vec3 normal = normalize(fs_in.Normal);

	vec3 result = CalcDirLight(normal, viewDir);
	if(useSpotLight)
		result += CalcSpotLight(normal, viewDir);

	if (usePointLight) {
		for (int i = 0; i < NUM_POINT_LIGHTS; ++i) {
			result += CalcPointLight(pointLight[i], normal, viewDir);
		}
	}
	
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
	vec3 lightDir = normalize(spotLight.position - fs_in.FragPos);

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

vec3 CalcPointLight(PointLight pointLight, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(pointLight.position - fs_in.FragPos);

	float diff = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	float distance = length(pointLight.position - fs_in.FragPos);
	float attenuation = 1.0 / (pointLight.constant + (pointLight.linear * distance) + (pointLight.quadratic * (distance * distance)));

	vec3 ambient = pointLight.ambient * material.ambient;
	vec3 diffuse = pointLight.diffuse * diff * material.diffuse;
	vec3 specular = pointLight.specular * spec * material.specular;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

float LinearizeDepth(float depth) {
	float ndc = depth * 2.0 - 1.0; 
	return (2.0 * near * far) / (far + near - ndc * (far - near));
}