#ifndef TEXTURE_H
#define TEXTURE_H

unsigned int createTexture(const char* texturePath);
void bindTexture(unsigned int texture, unsigned int index);

#endif