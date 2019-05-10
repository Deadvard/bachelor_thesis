#ifndef SHADER_H
#define SHADER_H
#include <glm/glm.hpp>

unsigned int createShader(const char* vertexPath, const char* fragmentPath);
unsigned int createShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath);
unsigned int createShader(const char* computePath);

void uniform(unsigned int shader, const char* name, bool value);
void uniform(unsigned int shader, const char* name, char* value, int size);
void uniform(unsigned int shader, const char* name, int value);
void uniform(unsigned int shader, const char* name, float value);
void uniform(unsigned int shader, const char* name, const glm::vec2& value);
void uniform(unsigned int shader, const char* name, const glm::vec3& value);
void uniform(unsigned int shader, const char* name, const glm::vec4& value);
void uniform(unsigned int shader, const char* name, const glm::mat4& value);

#endif