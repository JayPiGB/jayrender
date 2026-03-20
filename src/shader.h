#pragma once
#include<glad/glad.h>
struct Shader {
	GLuint program;
	char* loadShaderSourceFromFile(const char* path);
	bool buildShaderProgram(const char* vertexPath, const char* fragmentPath);
	void bind();
	void unbind();
};
