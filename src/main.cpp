#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "camera/camera.h"	
#include "data/cubes.h"
#include "data/light/light.h"
#include "data/material/material.h"
#include "shader/shader.h"
#include "resource_manager/resource_manager.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 400;

Camera cam(0.0f, 0.0f, 5.0f);
bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

float lastTime = 0.0f;
float deltaTime = 0.0f;

unsigned char autoLightMovement = false;

glm::vec3 lightPos(1.2f, 0.0f, 2.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_movement_callback(GLFWwindow* window, double xposIn, double yposIn);
void processKeyboardInput(GLFWwindow* window, float deltaTime);
void moveLightSource(GLFWwindow* window, float deltaTime);
void autoMoveLightSrc(float currentTime);

int main()
{
	if (!glfwInit()) return -1;

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "JayRenderer!", NULL, NULL);

	if (!window) return -1;

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_movement_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	unsigned int VBO, EBO, cubeVAO, lightSrcVAO;

	glGenBuffers(1, &VBO); 
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &cubeVAO);
	glGenVertexArrays(1, &lightSrcVAO);

	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	glBindVertexArray(lightSrcVAO);
	//same cube positions and attributes
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	Shader lightingShader = ResourceManager::LoadShader("resources/shaders/shader.vert", "resources/shaders/phong.frag", nullptr, "lightingShader");
	Shader lightSrcShader = ResourceManager::LoadShader("resources/shaders/shader.vert", "resources/shaders/shader.frag", nullptr, "lightSrcShader");

	Texture2D containerTexture = ResourceManager::LoadTexture("resources/textures/container2.png", true, "container");

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) 
			{ 
				if (key == GLFW_KEY_M && action == GLFW_PRESS)
				{
					autoLightMovement = !autoLightMovement; 
				}
			});

	while(!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);

		float currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (autoLightMovement) { autoMoveLightSrc(currentTime); }

		glm::mat4 model(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(SCR_WIDTH)/static_cast<float>(SCR_HEIGHT), 0.1f, 100.0f);
		glm::mat4 view = cam.getViewMatrix();

#pragma region LIGHT-SRC
		lightSrcShader.Use();

		glUniformMatrix4fv(glGetUniformLocation(lightSrcShader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(glGetUniformLocation(lightSrcShader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glUniformMatrix4fv(glGetUniformLocation(lightSrcShader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(lightSrcVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
#pragma endregion

#pragma region CUBE
		lightingShader.Use();

		glUniform3f(glGetUniformLocation(lightingShader.programId, "objectColor"), 1.0f, 0.5f, 0.31f);

		glUniform3f(glGetUniformLocation(lightingShader.programId, "lightColor"), 1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0f);

		glUniformMatrix4fv(glGetUniformLocation(lightingShader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glUniformMatrix4fv(glGetUniformLocation(lightingShader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glUniformMatrix4fv(glGetUniformLocation(lightingShader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		glUniform3f(glGetUniformLocation(lightingShader.programId, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

		glUniform3f(glGetUniformLocation(lightingShader.programId, "viewerPos"), cam.position.x, cam.position.y, cam.position.z); 

 		Material cubeMaterial(glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(1.0f, 0.5f, 0.31f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f);

		Light lighSource(lightPos, glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f));

		cubeMaterial.passMaterialToShader(lightingShader.programId);
		lighSource.passLightToShader(lightingShader.programId);
		containerTexture.Bind();

		glBindVertexArray(cubeVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
#pragma endregion

		glfwSwapBuffers(window);
		glfwPollEvents();
		processKeyboardInput(window, deltaTime);
	}

	return 0;
}

void autoMoveLightSrc(float currentTime)
{
	float radius = 1.2;
	// lightPos.y = radius * sin(currentTime);
	lightPos.x = radius * cos(currentTime);
	lightPos.z = radius * sin(currentTime);
}

void processKeyboardInput(GLFWwindow* window, float deltaTime)
{
	moveLightSource(window, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam.moveCamera(CAMERA_MOVEMENT::FRONT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam.moveCamera(CAMERA_MOVEMENT::BACK, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam.moveCamera(CAMERA_MOVEMENT::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam.moveCamera(CAMERA_MOVEMENT::RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
		cam.moveCamera(CAMERA_MOVEMENT::UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		cam.moveCamera(CAMERA_MOVEMENT::DOWN, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		cam.centerCamera();
	}

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

void moveLightSource(GLFWwindow* window, float deltaTime)
{
	float lightSrcSpeed = 2.0f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		lightPos += glm::vec3(0.0f, 0.0f, -1.0f) * lightSrcSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		lightPos += glm::vec3(-1.0f, 0.0f, 0.0f) * lightSrcSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		lightPos += glm::vec3(0.0f, 0.0f, 1.0f) * lightSrcSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		lightPos += glm::vec3(1.0f, 0.0f, 0.0f) * lightSrcSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		lightPos += glm::vec3(0.0f, 1.0f, 0.0f) * lightSrcSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		lightPos += glm::vec3(0.0f, -1.0f, 0.0f) * lightSrcSpeed;
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_movement_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = ypos - lastY;

	lastX = xpos;
	lastY = ypos;
	
	cam.processMouseMovement(xoffset, yoffset);
}
