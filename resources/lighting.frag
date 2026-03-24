#version 330 core

in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform mat3 normalMat;

out vec4 FragColor;

void main()
{
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(normalMat * Normal); 
	vec3 lightDirection = normalize(lightPos - FragPos);
	float diff = max(dot(lightDirection, norm), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 result = (ambient + diffuse) * objectColor;
	FragColor = vec4(result, 1.0);
}
