/*
 * glutils.h
 *
 *  Created on: Oct 22, 2013
 *      Author: nihathrael
 */

#ifndef GLUTILS_H_
#define GLUTILS_H_

#include <GL/glew.h>

class GLUtils {
public:
	GLUtils();
	virtual ~GLUtils();
	static bool bindAttribute(GLuint program, const char* attributeName,
			GLint& attribute);
	static bool bindUniform(GLuint program, const char* uniformName, GLint& uniform);
};

#endif /* GLUTILS_H_ */
