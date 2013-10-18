/*
 * objloader.cpp
 *
 *  Created on: Oct 17, 2013
 *      Author: nihathrael
 */

#include "objloader.h"

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <sstream>
#include <iostream>

// Include GLEW. Always include it before gl.h and glfw.h, since it's a bit magic.
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

ObjLoader::ObjLoader() {
	// TODO Auto-generated constructor stub

}

ObjLoader::~ObjLoader() {
	// TODO Auto-generated destructor stub
}

void ObjLoader::load_obj(const char* filename, Mesh* mesh) {
	ifstream in(filename, ios::in);
	if (!in) {
		cerr << "Cannot open " << filename << endl;
		exit(1);
	}

	string line;
	while (getline(in, line)) {
		if (line.substr(0, 2) == "v ") {
			istringstream s(line.substr(2));
			glm::vec4 v;
			s >> v.x;
			s >> v.y;
			s >> v.z;
			v.w = 1.0;
			mesh->vertices.push_back(v);
		} else if (line.substr(0, 2) == "f ") {
			istringstream s(line.substr(2));
			GLushort a, b, c;
			s >> a;
			s >> b;
			s >> c;
			a--;
			b--;
			c--;
			mesh->elements.push_back(a);
			mesh->elements.push_back(b);
			mesh->elements.push_back(c);
		} else if (line[0] == '#') { /* ignoring this line */
		} else { /* ignoring this line */
		}
	}

//	mesh->normals.resize(mesh->vertices.size(), glm::vec3(0.0, 0.0, 0.0));
//	vector<int> nb_seen;
//	nb_seen.resize(mesh->vertices.size(), 0);
//	for (unsigned int i = 0; i < mesh->elements.size(); i += 3) {
//		GLushort ia = mesh->elements[i];
//		GLushort ib = mesh->elements[i + 1];
//		GLushort ic = mesh->elements[i + 2];
//		glm::vec3 normal = glm::normalize(
//				glm::cross(
//						glm::vec3(mesh->vertices[ib])
//								- glm::vec3(mesh->vertices[ia]),
//						glm::vec3(mesh->vertices[ic])
//								- glm::vec3(mesh->vertices[ia])));
//		int v[3];
//		v[0] = ia;
//		v[1] = ib;
//		v[2] = ic;
//		for (int j = 0; j < 3; j++) {
//			GLushort cur_v = v[j];
//			nb_seen[cur_v]++;
//			if (nb_seen[cur_v] == 1) {
//				mesh->normals[cur_v] = normal;
//			} else {
//				// average
//				mesh->normals[cur_v].x = mesh->normals[cur_v].x
//						* (1.0 - 1.0 / nb_seen[cur_v])
//						+ normal.x * 1.0 / nb_seen[cur_v];
//				mesh->normals[cur_v].y = mesh->normals[cur_v].y
//						* (1.0 - 1.0 / nb_seen[cur_v])
//						+ normal.y * 1.0 / nb_seen[cur_v];
//				mesh->normals[cur_v].z = mesh->normals[cur_v].z
//						* (1.0 - 1.0 / nb_seen[cur_v])
//						+ normal.z * 1.0 / nb_seen[cur_v];
//				mesh->normals[cur_v] = glm::normalize(mesh->normals[cur_v]);
//			}
//		}
//	}
}
