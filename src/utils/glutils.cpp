/*
 * glutils.cpp
 *
 *  Created on: Oct 22, 2013
 *      Author: nihathrael
 */

#include "glutils.h"
#include <stdio.h>

GLUtils::GLUtils() {
	// TODO Auto-generated constructor stub

}

GLUtils::~GLUtils() {
	// TODO Auto-generated destructor stub
}

/**
 * Binds the specified attribute name to the given shader program.
 */
bool GLUtils::bindAttribute(GLuint program, const char* attributeName,
		GLint& attribute) {
	attribute = glGetAttribLocation(program, attributeName);
	if (attribute == -1) {
		fprintf(stderr, "Could not bind attribute %s\n", attributeName);
		return false;
	}
	return true;
}

bool GLUtils::bindUniform(GLuint program, const char* uniformName, GLint& uniform) {
	uniform = glGetUniformLocation(program, uniformName);
	if (uniform == -1) {
		fprintf(stderr, "Could not bind uniform %s\n", uniformName);
		return false;
	}
	return true;

}
