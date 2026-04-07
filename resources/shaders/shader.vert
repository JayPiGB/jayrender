#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightPos;

out vec2 TexCoords;
out vec3 Normal;
out vec3 NormalView;
out vec3 FragPos;
out vec3 FragPosView;
out vec3 LightPosView;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);

	mat3 normalMat = mat3(transpose(inverse(model)));
	Normal = normalize(normalMat * aNormal);
	FragPos = vec3(model * vec4(aPos, 1.0));
	
	mat3 normalMatView = mat3(transpose(inverse(view * model))); 
	NormalView = normalize(normalMatView * aNormal);
	FragPosView = vec3(view * model * vec4(aPos, 1.0));
	LightPosView = vec3(view * vec4(lightPos, 0.0));

	TexCoords = aTexCoords;
}
