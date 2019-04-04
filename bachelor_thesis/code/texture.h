#ifndef TEXTURE_H
#define TEXTURE_H

unsigned int GL_TEXTURE_2D;

unsigned int createTexture(const char* texturePath);
void bindTexture(unsigned int texture, unsigned int index, unsigned int type = GL_TEXTURE_2D);

#endif