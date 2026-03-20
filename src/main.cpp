#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "cubes.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 400;

int main()
{
	if (!glfwInit()) return -1;

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "JayRenderer!", NULL, NULL);

	if (!window) return -1;

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

	glEnable(GL_DEPTH_TEST);

	unsigned int VBO, EBO, cubeVAO;

	glGenBuffers(1, &VBO); 
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &cubeVAO);

	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 6, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	while(!glfwWindowShouldClose(window))
	{
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		glViewport(0, 0, width, height);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(cubeVAO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;
}
