//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c)2005 - 2016 Media Design School
//
//  File Name : ResourceManager.cpp
//  Description: ResourceManager implementation
//  Author:   Alex (Valeriy) Ruzhitskiy
//  Mail:   alexander.izmaylov@gmail.com
//

#include "ResourceManager.h"

#include <iostream>
#include <sstream>
#include <fstream>

// Library include
#include <SOIL/SOIL.h>

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;

/*
* Function - LoadShader(@param)
* Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* const GLchar *vShaderFile - path to vertex shader
* const GLchar *fShaderFile - path to fragment shader
* const GLchar *gShaderFile - path to geometry shader (if any)
* std::string name - name for the shader for future use
*/
Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name)
{
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

/*
* Function - GetShader(@param)
* Retrieves a stored sader
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* std::string name - name for the shader to retrieve
*/
Shader ResourceManager::GetShader(std::string name)
{
	return Shaders[name];
}

/*
* Function - LoadTexture(@param)
* Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* const GLchar *file - path to texture
* GLboolean alpha - does the texture use alpha channel?
* std::string name - name for the texture for future use
*/
Texture2D ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, std::string name)
{
	Textures[name] = loadTextureFromFile(file, alpha);
	return Textures[name];
}

/*
* Function - GetTexture(@param)
* Retrieves a stored sader
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* std::string name - name for the texture to retrieve
*/
Texture2D ResourceManager::GetTexture(std::string name)
{
	return Textures[name];
}


/*
* Function - Clear()
* Properly de-allocates all loaded resources
* Author - Alex (Valeriy) Ruzhitskiy
*/
void ResourceManager::Clear()
{
	// (Properly) delete all shaders	
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
	// (Properly) delete all textures
	for (auto iter : Textures)
		glDeleteTextures(1, &iter.second.ID);
}

/*
* Function - loadShaderFromFile(@param)
* Helper function for loading shader
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* const GLchar *vShaderFile - path to vertex shader
* const GLchar *fShaderFile - path to fragment shader
* const GLchar *gShaderFile - path to geometry shader (if any)
*/
Shader ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile)
{
	// 1. Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	try
	{
		// Open files
		std::ifstream vertexShaderFile(vShaderFile);
		std::ifstream fragmentShaderFile(fShaderFile);
		std::stringstream vShaderStream, fShaderStream;
		// Read file's buffer contents into streams
		vShaderStream << vertexShaderFile.rdbuf();
		fShaderStream << fragmentShaderFile.rdbuf();
		// close file handlers
		vertexShaderFile.close();
		fragmentShaderFile.close();
		// Convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
		// If geometry shader path is present, also load a geometry shader
		if (gShaderFile != nullptr)
		{
			std::ifstream geometryShaderFile(gShaderFile);
			std::stringstream gShaderStream;
			gShaderStream << geometryShaderFile.rdbuf();
			geometryShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::exception e)
	{
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = geometryCode.c_str();
	// 2. Now create shader object from source code
	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
	return shader;
}

/*
* Function - loadTextureFromFile(@param)
* Helper function to for LoadTexture()
* Author - Alex (Valeriy) Ruzhitskiy
* @param :
* const GLchar *file - path to texture
* GLboolean alpha - does the texture use alpha channel?
*/
Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha)
{
	// Create Texture object
	Texture2D texture;
	if (alpha)
	{
		texture.Internal_Format = GL_RGBA;
		texture.Image_Format = GL_RGBA;
	}
	// Load image
	int width, height;
	unsigned char* image = SOIL_load_image(file, &width, &height, 0, texture.Image_Format == GL_RGBA ? SOIL_LOAD_RGBA : SOIL_LOAD_RGB);
	// Now generate texture
	texture.Generate(width, height, image);
	// And finally free image data
	SOIL_free_image_data(image);
	return texture;
}