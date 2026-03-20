#include "shader.h"
#include <iostream>
#include <fstream>
#include <string>

#include <filesystem>

void Shader::bind() {
	glUseProgram(program);
}

void Shader::unbind() {
	glDeleteProgram(program);

	*this = {};
}

char* Shader::loadShaderSourceFromFile(const char* path) {
	std::ifstream sourceFile;
	sourceFile.open(path);

	std::filesystem::path exePath = std::filesystem::current_path();
	std::cout << exePath << std::endl;

	if (!sourceFile.is_open()) {
		std::cout << "Error opening shader source file!" << std::endl;
	}

	int size;
	sourceFile.seekg(0, sourceFile.end);
	size = sourceFile.tellg();
	sourceFile.seekg(0, sourceFile.beg);

	char* content = new char[size] {};
	sourceFile.read(content, size);

	sourceFile.close();

	return content;
}

bool Shader::buildShaderProgram(const char* vertexPath, const char* fragmentPath) {
	 program = glCreateProgram();

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexSource = loadShaderSourceFromFile(vertexPath);
	glShaderSource(vertexShader, 1, &vertexSource, nullptr);
	glCompileShader(vertexShader);

	GLint compilationStatus;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compilationStatus);
	if (compilationStatus == GL_FALSE) {
		std::cout << "Error compiling VERTEX shader source code" << std::endl;
		
		GLint logSize;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logSize);
		char* logBuff = new char [logSize];
		glGetShaderInfoLog(vertexShader, sizeof(logBuff), &logSize, logBuff);
		std::cout << logBuff << std::endl;

		return false;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentSource = loadShaderSourceFromFile(fragmentPath);
	glShaderSource(fragmentShader, 1, &fragmentSource, nullptr);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compilationStatus);
	if (compilationStatus == GL_FALSE) {
		std::cout << "Error compiling FRAGMENT shader source code" << std::endl;

		GLint logSize;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logSize);
		char* logBuff = new char[logSize];
		glGetShaderInfoLog(fragmentShader, sizeof(logBuff), &logSize, logBuff);
		std::cout << logBuff << std::endl;
		
		return false;
	}

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE) {
		std::cout << "Error linking the shader program!" << std::endl;

		GLint logSize;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
		char* logBuff = new char[logSize];
		glGetProgramInfoLog(program, sizeof(logBuff), &logSize, logBuff);
		std::cout << logBuff << std::endl;

		return false;
	}

	return true;
}