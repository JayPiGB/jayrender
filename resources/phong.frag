#version 330 core

in vec3 Normal;
in vec3 FragPos;

uniform float ambientStr;
uniform float specularStr;
uniform int shininess;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewerPos;

out vec4 FragColor;

void main()
{
	vec3 ambient = ambientStr * lightColor;

	vec3 lightDirection = normalize(lightPos - FragPos);
	float diff = max(dot(lightDirection, Normal), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 reflectDirection = reflect(-lightDirection, Normal);
	vec3 viewerDirection = normalize(viewerPos - FragPos);
	float spec = pow(max(dot(reflectDirection, viewerDirection), 0), shininess);

	vec3 specular = specularStr * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1.0);
}
