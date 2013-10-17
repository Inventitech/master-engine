/*
 * objloader.h
 *
 *  Created on: Oct 17, 2013
 *      Author: nihathrael
 */

#ifndef OBJLOADER_H_
#define OBJLOADER_H_

#include "mesh.h"

class ObjLoader {
public:
	ObjLoader();
	virtual ~ObjLoader();
	void load_obj(const char* filename, Mesh* mesh);
};

#endif /* OBJLOADER_H_ */
