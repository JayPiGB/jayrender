#version 330 core

in vec3 NormalView; //Normal in view space
in vec3 FragPosView; //Fragment position in view space
in vec3 LightPosView; //Light source position in view space

uniform float ambientStr;
uniform float specularStr;
uniform int shininess;

uniform vec3 objectColor;
uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
	vec3 ambient = ambientStr * lightColor;

	vec3 lightDir = normalize(LightPosView - FragPosView);
	float diff = max(dot(NormalView, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	vec3 reflectDir = reflect(-lightDir, NormalView);
	vec3 viewerDir = normalize(-FragPosView); //since the viewer is always at the origin in view space, (0.0, 0.0, 0.0) - FragPosView = -FragPosView
	float spec = pow(max(dot(reflectDir, viewerDir), 0.0), shininess);

	vec3 specular = specularStr * spec * lightColor;

	vec3 result = (ambient + diffuse + specular) * objectColor;

	FragColor = vec4(result, 1.0);

}
