#include "light.h"
#include "glad/glad.h"

Light::Light(glm::vec3 position,
		glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular) :
	position(position),
	ambient(ambient),
	diffuse(diffuse),
	specular(specular) {}

void Light::passLightToShader(unsigned int programId)
{
	glUniform3f(glGetUniformLocation(programId, "light.position"), position.x, position.y, position.z);
	glUniform3f(glGetUniformLocation(programId, "light.ambient"), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(programId, "light.diffuse"), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(programId, "light.specular"), specular.x, specular.y, specular.z);
}

