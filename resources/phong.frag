#version 330 core

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewerPos;

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};
uniform Material material;

struct Light
{
	vec3 position;
	//these are the light colors of the reflection for each lighting component
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Light light;


out vec4 FragColor;

void main()
{
	vec3 ambient = light.ambient * material.ambient;

	vec3 lightDirection = normalize(lightPos - FragPos);
	float diff = max(dot(lightDirection, Normal), 0.0);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	vec3 reflectDirection = reflect(-lightDirection, Normal);
	vec3 viewerDirection = normalize(viewerPos - FragPos);
	float spec = pow(max(dot(reflectDirection, viewerDirection), 0), material.shininess);

	vec3 specular = light.specular * (spec * material.specular);

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
}
