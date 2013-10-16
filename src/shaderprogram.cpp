/*
 * ShaderProgram.cpp
 *
 *  Created on: Oct 16, 2013
 *      Author: nihathrael
 */

#include <stdio.h>

#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string.h>

#include "shaderprogram.h"

ShaderProgram::ShaderProgram() :
		vertexShaderFile(NULL), fragmentShaderFile(NULL), program(-1) {
}

ShaderProgram::ShaderProgram(const char* vertexShaderFile,
		const char* fragmentShaderFile) :
		vertexShaderFile(vertexShaderFile), fragmentShaderFile(
				fragmentShaderFile) {
	compile();
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(this->program);
}

std::string ShaderProgram::loadShaderContent(const char* file) {
	std::string content;
	std::ifstream inputStream(file, std::ios::in);
	if (inputStream.is_open()) {
		std::string Line = "";
		while (getline(inputStream, Line)) {
			content += "\n" + Line;
		}
		inputStream.close();
	} else {
		printf("Impossible to open %s.\n", file);
		return NULL;

	}
	return content;
}

bool ShaderProgram::compileShader(const std::string& shaderCode,
		GLuint shaderID) {
	int infoLogLength;
	GLint result = GL_FALSE;
	const char* sourcePointer = shaderCode.c_str();
	glShaderSource(shaderID, 1, &sourcePointer, NULL);
	glCompileShader(shaderID);
	// Check Shader
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (!result) {
		std::vector<char> vertexShaderErrorMessage(infoLogLength + 1);
		glGetShaderInfoLog(shaderID, infoLogLength, NULL,
				&vertexShaderErrorMessage[0]);
		printf("%s\n", &vertexShaderErrorMessage[0]);
		return false;
	}
	return true;
}

GLuint ShaderProgram::compile() {
	this->program = -1;

	// Make sure both shaders are set
	if (this->fragmentShaderFile == NULL) {
		printf("Fragmentshader file not set!");
		return 0;
	}
	if (this->vertexShaderFile == NULL) {
		printf("Vertexshader file not set!");
		return 0;
	}

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode = this->loadShaderContent(this->vertexShaderFile);
	std::string FragmentShaderCode = this->loadShaderContent(this->fragmentShaderFile);


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", this->vertexShaderFile);
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	bool vertexShaderCompiled = compileShader(VertexShaderCode, vertexShaderID);

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", this->fragmentShaderFile);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	bool fragmentShaderCompiled = compileShader(FragmentShaderCode, fragmentShaderID);

	if(!vertexShaderCompiled || !fragmentShaderCompiled) {
		return 0;
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vertexShaderID);
	glAttachShader(ProgramID, fragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	int infoLogLength;
	GLint result = GL_FALSE;
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (!result) {
		std::vector<char> ProgramErrorMessage(infoLogLength + 1);
		glGetProgramInfoLog(ProgramID, infoLogLength, NULL,
				&ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
	printf("Done\n");

	this->program = ProgramID;
	return ProgramID;
}
