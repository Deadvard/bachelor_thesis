#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <stb_image.h>
#include <iostream>

unsigned int createTexture(const char* texturePath)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	stbi_set_flip_vertically_on_load(true);

	int width, height, numComponents;

	unsigned char* data = stbi_load(texturePath, &width, &height, &numComponents, 4);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width,
			height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	else
	{
		std::cout << "Could not load texture from path: " << texturePath << "\n";
	}

	return textureID;
}

void bindTexture(unsigned int texture, unsigned int index, unsigned int type)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(type, texture);
}
