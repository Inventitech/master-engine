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
		vertexShaderFile(NULL), fragmentShaderFile(NULL) {
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

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode = this->loadShaderContent(this->vertexShaderFile);
	std::string FragmentShaderCode = this->loadShaderContent(this->fragmentShaderFile);

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", this->fragmentShaderFile);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL,
				&VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", this->fragmentShaderFile);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL,
				&FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL,
				&ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
	printf("Done\n");

	this->program = ProgramID;
	return ProgramID;
}
