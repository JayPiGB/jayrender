#pragma once

#include <map>
#include <string>

#include <glad/glad.h>

#include "../Texture/texture.h"
#include "../Shader/shader.h"

// A static singleton ResourceManager class that hosts several
// functions to load Textures and Shaders. Each loaded texture
// and/or shader is also stored for future reference by string
// handles. All functions and resources are static and no 
// public constructor is defined.
class ResourceManager {
public:
	//resource storage
	static std::map<std::string, Shader> Shaders;
	static std::map<std::string, Texture2D> Textures;

	// loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If geometryShaderPath is not nullptr, it also loads a geometry shader
	static Shader& LoadShader(const char* vertexShaderPath, const char* fragmentShaderPath, const char* geometryShaderPath, std::string name);
	//retrieves stored shader
	static Shader& GetShader(std::string name);

	//loads (and generates) a texture from file
	static Texture2D& LoadTexture(const char* file, bool alpha, std::string name);
	//retrieves stored texture
	static Texture2D& GetTexture(std::string name);

	//properly de-allocates all loaded resources
	static void Clear();

private:
	//private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
	ResourceManager() { }

	//loads and generates a shader from file
	static Shader& loadShaderFromFile(const char* vertexShaderFile, const char* fragmentShaderFile, const char* geometryShaderFile = nullptr);

	//loads a single texture from file
	static Texture2D& loadTextureFromFile(const char* file, bool alpha);
};
