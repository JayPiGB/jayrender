#include "shader.h"

#include <iostream>

#include <glad/glad.h>

//helper functions
static GLenum getShaderTypeGLEnum(SHADER_COMPILATION_TYPES type);
static const char* getShaderTypeName(SHADER_COMPILATION_TYPES type);
static void checkCompileErrors(unsigned int objectId, SHADER_COMPILATION_TYPES type);
static unsigned int compileShader(const char* source, SHADER_COMPILATION_TYPES type);

Shader& Shader::Use() {
	glUseProgram(this->programId);
	return *this;
}

void Shader::Build(const char* vertexSource, const char* fragmentSource, const char* geometrySource) {
	unsigned int vertexShader, fragmentShader, geometryShader;
	
	vertexShader = compileShader(vertexSource, VERTEX);
	fragmentShader = compileShader(fragmentSource, FRAGMENT);
	if (geometrySource != nullptr) {
		geometryShader = compileShader(geometrySource, GEOMETRY);
	}

	this->programId = glCreateProgram();
	glAttachShader(this->programId, vertexShader);
	glAttachShader(this->programId, fragmentShader);
	if (geometrySource != nullptr) {
		glAttachShader(this->programId, geometryShader);
	}
	glLinkProgram(this->programId);
	checkCompileErrors(this->programId, PROGRAM);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	if (geometrySource != nullptr) {
		glDeleteShader(geometryShader);
	}
}

void Shader::SetFloat(const char* name, float value, bool useShader) {
	if (useShader)
		this->Use();
	glUniform1f(glGetUniformLocation(this->programId, name), value);
}
void Shader::SetInteger(const char* name, int value, bool useShader) {
	if (useShader)
		this->Use();
	glUniform1i(glGetUniformLocation(this->programId, name), value);
}
void Shader::SetVector2f(const char* name, float x, float y, bool useShader) {
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->programId, name), x, y);
}
void Shader::SetVector2f(const char* name, const glm::vec2& value, bool useShader) {
	if (useShader)
		this->Use();
	glUniform2f(glGetUniformLocation(this->programId, name), value.x, value.y);
}
void Shader::SetVector3f(const char* name, float x, float y, float z, bool useShader) {
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->programId, name), x, y, z);
}
void Shader::SetVector3f(const char* name, const glm::vec3& value, bool useShader) {
	if (useShader)
		this->Use();
	glUniform3f(glGetUniformLocation(this->programId, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char* name, float x, float y, float z, float w, bool useShader) {
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->programId, name), x, y, z, w);
}
void Shader::SetVector4f(const char* name, const glm::vec4& value, bool useShader) {
	if (useShader)
		this->Use();
	glUniform4f(glGetUniformLocation(this->programId, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char* name, const glm::mat4& matrix, bool useShader) {
	if (useShader)
		this->Use();
	glUniformMatrix4fv(glGetUniformLocation(this->programId, name), 1, false, glm::value_ptr(matrix));
}

static void checkCompileErrors(unsigned int objectId, SHADER_COMPILATION_TYPES type) {
	if (type == PROGRAM) {
		GLint linkStatus = 0;
		glGetProgramiv(objectId, GL_LINK_STATUS, &linkStatus);
		if (!linkStatus) {
			GLint logLength = 0;
			glGetProgramiv(objectId, GL_INFO_LOG_LENGTH, &logLength);

			char* log = new char[logLength];
			glGetProgramInfoLog(objectId, logLength, nullptr, log);

			std::cout << "Linking failed for shader " << getShaderTypeName(type) << ": " << log << std::endl;
		}
	}
	else {
		GLint compilationStatus = 0;
		glGetShaderiv(objectId, GL_COMPILE_STATUS, &compilationStatus);
		if (!compilationStatus) {
			GLint logLength = 0;
			glGetShaderiv(objectId, GL_INFO_LOG_LENGTH, &logLength);

			char* log = new char[logLength];
			glGetShaderInfoLog(objectId, logLength, nullptr, log);

			std::cout << "Compilation failed for shader " << getShaderTypeName(type) << ": " << log << std::endl;
		}
	}
}

static unsigned int compileShader(const char* source, SHADER_COMPILATION_TYPES type) {
	unsigned int shaderId = glCreateShader(getShaderTypeGLEnum(type));
	glShaderSource(shaderId, 1, &source, NULL);
	glCompileShader(shaderId);

	checkCompileErrors(shaderId, type);
	return shaderId;
}

static GLenum getShaderTypeGLEnum(SHADER_COMPILATION_TYPES type) {
	switch (type) {
	case PROGRAM:
		return GL_PROGRAM;
	case VERTEX:
		return GL_VERTEX_SHADER;
	case FRAGMENT:
		return GL_FRAGMENT_SHADER;
	case GEOMETRY:
		return GL_GEOMETRY_SHADER;
	default:
		return 0;
	}
}

static const char* getShaderTypeName(SHADER_COMPILATION_TYPES type) {
	switch (type) {
	case PROGRAM:
		return "PROGRAM";
	case VERTEX:
		return "VERTEX";

	case FRAGMENT:
		return "FRAGMENT";
	case GEOMETRY:
		return "GEOMETRY";
	default:
		return 0;
	}
}
