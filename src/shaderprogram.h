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

	GLuint program;
public:
	ShaderProgram();
	ShaderProgram(const char* vertexShaderFile, const char* fragmentShaderFile);
	virtual ~ShaderProgram();

	GLuint compile();

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
};

#endif /* SHADERPROGRAM_H_ */
