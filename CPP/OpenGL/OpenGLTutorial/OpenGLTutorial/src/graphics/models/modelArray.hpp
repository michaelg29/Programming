//#ifndef MODELARRAY_HPP
//#define MODELARRAY_HPP
//
//#include "../model.h"
//#include "box.hpp"
//#include <cmath>
//
//#define UPPER_BOUND 100
//
//template <class T>
//class ModelArray {
//public:
//	std::vector<RigidBody> instances;
//
//	void init() {
//		model.init();
//
//		// generate positions VBO
//		posVBO = BufferObject(GL_ARRAY_BUFFER);
//		posVBO.generate();
//		posVBO.bind();
//		posVBO.setData<glm::vec3>(UPPER_BOUND, NULL, GL_DYNAMIC_DRAW);
//
//		// generate size VBO - dynamic
//		sizeVBO = BufferObject(GL_ARRAY_BUFFER);
//		sizeVBO.generate();
//		sizeVBO.bind();
//		sizeVBO.setData<glm::vec3>(UPPER_BOUND, NULL, GL_DYNAMIC_DRAW);
//
//		for (unsigned int i = 0, size = model.meshes.size(); i < size; i++) {
//			model.meshes[i].VAO.bind();
//
//			// set the vertex attribute pointers
//			// positions
//			posVBO.bind();
//			posVBO.setAttPointer<glm::vec3>(3, 3, GL_FLOAT, 1, 0, 1);
//			// size
//			sizeVBO.bind();
//			sizeVBO.setAttPointer<glm::vec3>(4, 3, GL_FLOAT, 1, 0, 1);
//
//			ArrayObject::clear();
//		}
//	}
//
//	void render(Shader shader, float dt, Box *box, bool setLists = true) {
//		if (setLists) {
//			positions.clear();
//			sizes.clear();
//			for (RigidBody& rb : instances) {
//				rb.update(dt);
//				positions.push_back(rb.pos);
//				sizes.push_back(model.size);
//			}
//		}
//
//		model.render(shader, dt, box, false, false);
//
//		int instances = std::min(100, (int)positions.size());
//
//		if (instances != 0) {
//			// reset VBO data
//			posVBO.bind();
//			posVBO.updateData<glm::vec3>(0, instances, &positions[0]);
//
//			sizeVBO.bind();
//			sizeVBO.updateData<glm::vec3>(0, instances, &sizes[0]);
//
//			ArrayObject::clear();
//		}
//
//		for (unsigned int i = 0, noMeshes = model.meshes.size(); i < noMeshes; i++) {
//			for (int j = 0, noInstances = positions.size(); j < noInstances; j++) {
//				box->pushInstance(model.meshes[i].br, positions[j], sizes[j]);
//			}
//
//			model.meshes[i].VAO.bind();
//			model.meshes[i].VAO.draw(GL_TRIANGLES, model.meshes[i].indices.size(), GL_UNSIGNED_INT, 0, instances);
//			ArrayObject::clear();
//		}
//	}
//
//	void setSize(glm::vec3 size) {
//		model.size = size;
//	}
//
//	void cleanup() {
//		model.cleanup();
//	}
//
//protected:
//	T model;
//
//	BufferObject posVBO;
//	BufferObject sizeVBO;
//	std::vector<glm::vec3> positions;
//	std::vector<glm::vec3> sizes;
//};
//
//#endif