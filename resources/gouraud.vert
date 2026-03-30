//Gourad shading in view space
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float ambientStr;
uniform float specularStr;
uniform int shininess;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewerPos;

out vec3 LightingColor;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	vec3 pos = vec3(model * vec4(aPos, 1.0));

	mat3 normalMat = mat3(transpose(inverse(model)));
	vec3 normal = normalize(normalMat * aNormal);

	vec3 ambientLight = ambientStr * lightColor;

	vec3 lightDir = normalize(lightPos - pos);
	float diffuse = max(dot(lightDir, normal) , 0.0);
	vec3 diffuseLight = diffuse * lightColor;

	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 viewDir = normalize(viewerPos - normal);

	float specular = pow(max(dot(reflectDir, viewDir), 0.0), shininess);
	vec3 specularLight = specularStr * specular * lightColor;
	
	LightingColor = ambientLight + diffuseLight + specularLight;
}
