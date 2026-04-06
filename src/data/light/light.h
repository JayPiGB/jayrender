#include <glm.hpp>

class Light
{
public:
	Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);

	void passLightToShader(unsigned int programId);
	 
private:
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};
