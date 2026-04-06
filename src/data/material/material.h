#include <glm.hpp>	

class Material
{
public:
	Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

	void passMaterialToShader(unsigned int programId);
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
};
