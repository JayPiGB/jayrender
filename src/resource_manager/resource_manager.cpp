#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//instantiate static variables
std::map<std::string, Texture2D> ResourceManager::Textures;
std::map<std::string, Shader> ResourceManager::Shaders;

//review error handling
Shader& ResourceManager::GetShader(std::string name) {
	return Shaders.at(name);
}

//review error handling
Texture2D& ResourceManager::GetTexture(std::string name) {
	return Textures.at(name);
}

Texture2D& ResourceManager::LoadTexture(const char* file, bool alpha, std::string name) {
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

Shader& ResourceManager::LoadShader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath, std::string name) {
	Shaders[name] = loadShaderFromFile(vertexShaderPath, fragmentShaderPath, geometryShaderPath);
	return Shaders[name];
}

Texture2D& ResourceManager::loadTextureFromFile(const char* file, bool alpha) {
	Texture2D* texture = new Texture2D();
	if (alpha) {
		texture->Internal_Format = GL_RGBA;
		texture->Image_Format = GL_RGBA;
	}

	int width, height, nrChannels;
	unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);

	texture->Generate(width, height, data);
	stbi_image_free(data);

	return *texture;
}

Shader& ResourceManager::loadShaderFromFile(const char* vertexSourcePath, const char* fragmentSourcePath, const char* geometrySourcePath) {
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;

	//todo: review where exceptions are really thrown and refactor error handling for streams
	try {
		std::ifstream vertexShaderFile(vertexSourcePath);
		std::ifstream fragmentShaderFile(fragmentSourcePath);

		std::stringstream vertexShStream, fragmentShStream;
		vertexShStream << vertexShaderFile.rdbuf();
		fragmentShStream << fragmentShaderFile.rdbuf();

		vertexShaderFile.close();
		fragmentShaderFile.close();

		vertexCode = vertexShStream.str();
		fragmentCode = fragmentShStream.str();

		if (geometrySourcePath != nullptr)
		{
			std::ifstream geometryShaderFile(geometrySourcePath);
			std::stringstream geometryShStream;
			geometryShStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = geometryShStream.str();
		}

	}
	catch (std::exception e) {
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	const char* gShaderCode = geometryCode.c_str();
	Shader* shader = new Shader();
	shader->Build(vShaderCode, fShaderCode, geometrySourcePath != nullptr ? gShaderCode : nullptr);
	return *shader;
}

//review iterators
void ResourceManager::Clear() {
	for (auto iter : Shaders) {
		glDeleteProgram(iter.second.programId);
	}
	for (auto iter : Textures) {
		glDeleteTextures(1, &iter.second.TextureId);
	}
}
