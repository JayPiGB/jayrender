#include "material.h"
#include "glad/glad.h"

Material::Material(glm::vec3 ambient,
		glm::vec3 diffuse,
		glm::vec3 specular,
		float shininess) :
	ambient(ambient),
	diffuse(diffuse), 
	specular(specular),
	shininess(shininess) {}

void Material::passMaterialToShader(unsigned int programId)
{
	glUniform3f(glGetUniformLocation(programId, "material.ambient"), ambient.x, ambient.y, ambient.z);
	glUniform3f(glGetUniformLocation(programId, "material.diffuse"), diffuse.x, diffuse.y, diffuse.z);
	glUniform3f(glGetUniformLocation(programId, "material.specular"), diffuse.x, diffuse.y, diffuse.z);
	glUniform1f(glGetUniformLocation(programId, "material.shininess"), shininess);
}
