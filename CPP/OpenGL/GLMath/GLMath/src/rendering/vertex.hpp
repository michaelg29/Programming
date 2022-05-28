#include <glm/glm.hpp>

#ifndef VERTEX_HPP
#define VERTEX_HPP

struct Vertex {
	glm::vec3 position;
	glm::vec2 texCoord;
	glm::vec3 normal;
};

#endif