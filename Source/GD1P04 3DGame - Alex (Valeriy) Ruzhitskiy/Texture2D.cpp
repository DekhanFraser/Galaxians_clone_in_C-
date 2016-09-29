//
//  Bachelor of Software Engineering
//  Media Design School
//  Auckland
//  New Zealand
//
//  (c)2005 - 2016 Media Design School
//
//  File Name : Texture2D.cpp
//  Description: Texture2D implementation
//  Author:   Alex (Valeriy) Ruzhitskiy
//  Mail:   alexander.izmaylov@gmail.com
//

#include <iostream>

#include "Texture2D.h"

/*
* Function - Texture2D()
* Default constructor
* Author:   Alex (Valeriy) Ruzhitskiy
*/
Texture2D::Texture2D()
	: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}

/*
* Function - Generate(@param)
* Generates texture from image data
* Author:   Alex (Valeriy) Ruzhitskiy
* @param :
* GLuint width - texure width
* GLuint height - texture height
* unsigned char* data
*/
void Texture2D::Generate(GLuint width, GLuint height, unsigned char* data)
{
	this->Width = width;
	this->Height = height;
	// Create Texture
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	// Set Texture wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

/*
* Function - Bind()
* Bind texture for rendering
* Author:   Alex (Valeriy) Ruzhitskiy
*/
void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}