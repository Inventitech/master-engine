/*
 * ShaderProgram.h
 *
 *  Created on: Oct 16, 2013
 *      Author: nihathrael
 */

#ifndef SHADERPROGRAM_H_
#define SHADERPROGRAM_H_

#include <string>
#include <GL/glew.h>

class ShaderProgram {
	const char * vertexShaderFile;
	const char * fragmentShaderFile;
	std::string loadShaderContent(const char* file);
	bool compileShader(const std::string&, GLuint);
	GLint attribute_coord3d, attribute_normal;
	GLint uniform_m, uniform_v, uniform_p;

	GLuint program;
public:
	ShaderProgram();
	ShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile);
	virtual ~ShaderProgram();

	GLuint compile();
	bool bind();

	const char* getFragmentShaderFile() const {
		return fragmentShaderFile;
	}

	void setFragmentShaderFile(const char* fragmentShaderFile) {
		this->fragmentShaderFile = fragmentShaderFile;
	}

	const char* getVertexShaderFile() const {
		return vertexShaderFile;
	}

	void setVertexShaderFile(const char* vertexShaderFile) {
		this->vertexShaderFile = vertexShaderFile;
	}

	/*
	 * @returns: the shader program's id, or -1 if not set or an error occurred during the compile
	 */
	GLuint getProgram() const {
		return program;
	}

	GLint getAttributeCoord3d() const {
		return attribute_coord3d;
	}

	GLint getAttributeNormal() const {
		return attribute_normal;
	}

	GLint getUniformM() const {
		return uniform_m;
	}

	GLint getUniformP() const {
		return uniform_p;
	}

	GLint getUniformV() const {
		return uniform_v;
	}
};

#endif /* SHADERPROGRAM_H_ */
