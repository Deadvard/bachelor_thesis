#include "shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

void checkCompileErrors(unsigned int shader, const char* type);

void createShader(unsigned int* shader, const char* vertexPath, const char* fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	// 2. compile shaders
	unsigned int vertex, fragment;
	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	// shader Program
	*shader = glCreateProgram();
	glAttachShader(*shader, vertex);
	glAttachShader(*shader, fragment);
	glLinkProgram(*shader);
	checkCompileErrors(*shader, "PROGRAM");
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void uniform(unsigned int shader, const char* name, bool value)
{
	glUniform1i(glGetUniformLocation(shader, name), (int)value);
}

void uniform(unsigned int shader, const char* name, int value)
{
	glUniform1i(glGetUniformLocation(shader, name), value);
}

void uniform(unsigned int shader, const char* name, float value)
{
	glUniform1f(glGetUniformLocation(shader, name), value);
}

void uniform(unsigned int shader, const char* name, const glm::vec2 &value)
{
	glUniform2fv(glGetUniformLocation(shader, name), 1, &value[0]);
}

void uniform(unsigned int shader, const char* name, const glm::vec3 &value)
{
	glUniform3fv(glGetUniformLocation(shader, name), 1, &value[0]);
}

void uniform(unsigned int shader, const char* name, const glm::vec4 &value)
{
	glUniform4fv(glGetUniformLocation(shader, name), 1, &value[0]);
}

void uniform(unsigned int shader, const char* name, const glm::mat4& mat)
{
	glUniformMatrix4fv(glGetUniformLocation(shader, name),
		1, GL_FALSE, glm::value_ptr(mat));
}

void checkCompileErrors(unsigned int shader, const char* type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
}