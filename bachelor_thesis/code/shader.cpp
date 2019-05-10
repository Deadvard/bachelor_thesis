#include "shader.h"

#include <glad/glad.h>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

void checkCompileErrors(unsigned int shader, const char* type);

unsigned int createShader(const char* vertexPath, const char* fragmentPath)
{	
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// open files
	vShaderFile.open(vertexPath);
	fShaderFile.open(fragmentPath);
	std::stringstream vShaderStream;
	std::stringstream fShaderStream;
	
	// read file's buffer contents into streams
	vShaderStream << vShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();
	
	// close file handlers
	vShaderFile.close();
	fShaderFile.close();
	
	// convert stream into string
	vertexCode = vShaderStream.str();
	fragmentCode = fShaderStream.str();
	
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();
	
	// vertex shader
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");
	
	// fragment Shader
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");
	
	// shader Program
	unsigned int shader = glCreateProgram();
	glAttachShader(shader, vertex);
	glAttachShader(shader, fragment);
	glLinkProgram(shader);
	checkCompileErrors(shader, "PROGRAM");
	
	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return shader;
}

unsigned int createShader(const char* vertexPath, const char* geometryPath, const char* fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string geometryCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream gShaderFile;
	std::ifstream fShaderFile;

	// open files
	vShaderFile.open(vertexPath);
	gShaderFile.open(geometryPath);
	fShaderFile.open(fragmentPath);
	std::stringstream vShaderStream;
	std::stringstream gShaderStream;
	std::stringstream fShaderStream;

	// read file's buffer contents into streams
	vShaderStream << vShaderFile.rdbuf();
	gShaderStream << gShaderFile.rdbuf();
	fShaderStream << fShaderFile.rdbuf();

	// close file handlers
	vShaderFile.close();
	gShaderFile.close();
	fShaderFile.close();

	// convert stream into string
	vertexCode = vShaderStream.str();
	geometryCode = gShaderStream.str();
	fragmentCode = fShaderStream.str();

	const char* vShaderCode = vertexCode.c_str();
	const char* gShaderCode = geometryCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// vertex shader
	unsigned int vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// geometry shader
	unsigned int geometry = glCreateShader(GL_GEOMETRY_SHADER);
	glShaderSource(geometry, 1, &gShaderCode, NULL);
	glCompileShader(geometry);
	checkCompileErrors(geometry, "GEOMETRY");

	// fragment Shader
	unsigned int fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// shader Program
	unsigned int shader = glCreateProgram();
	glAttachShader(shader, vertex);
	glAttachShader(shader, geometry);
	glAttachShader(shader, fragment);
	glLinkProgram(shader);
	checkCompileErrors(shader, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(geometry);
	glDeleteShader(fragment);

	return shader;
}

unsigned int createShader(const char* computePath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string computeCode;
	std::ifstream cShaderFile;

	// open files
	cShaderFile.open(computePath);
	std::stringstream cShaderStream;

	// read file's buffer contents into streams
	cShaderStream << cShaderFile.rdbuf();

	// close file handlers
	cShaderFile.close();

	// convert stream into string
	computeCode = cShaderStream.str();

	const char* cShaderCode = computeCode.c_str();

	// vertex shader
	unsigned int compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &cShaderCode, NULL);
	glCompileShader(compute);
	checkCompileErrors(compute, "COMPUTE");

	// shader Program
	unsigned int shader = glCreateProgram();
	glAttachShader(shader, compute);
	glLinkProgram(shader);
	checkCompileErrors(shader, "PROGRAM");

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(compute);

	return shader;
}

void uniform(unsigned int shader, const char* name, bool value)
{
	glUniform1i(glGetUniformLocation(shader, name), (int)value);
}

void uniform(unsigned int shader, const char * name, char * value, int size)
{

}

void uniform(unsigned int shader, const char* name, int value)
{
	glUniform1i(glGetUniformLocation(shader, name), value);
}

void uniform(unsigned int shader, const char* name, float value)
{
	glUniform1f(glGetUniformLocation(shader, name), value);
}

void uniform(unsigned int shader, const char* name, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(shader, name), 1, &value[0]);
}

void uniform(unsigned int shader, const char* name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(shader, name), 1, &value[0]);
}

void uniform(unsigned int shader, const char* name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(shader, name), 1, &value[0]);
}

void uniform(unsigned int shader, const char* name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(shader, name),
		1, GL_FALSE, glm::value_ptr(value));
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
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << 
				type << "\n" << infoLog << 
				"\n -- --------------------------------------------------- -- \n";
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " 
				<< type << "\n" << infoLog << 
				"\n -- --------------------------------------------------- -- \n";
		}
	}
}