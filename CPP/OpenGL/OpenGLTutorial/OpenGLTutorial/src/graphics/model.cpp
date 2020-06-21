#include "model.h"
#include "../scene.h"

Model::Model(std::string id, BoundTypes boundType, int maxNoInstances, unsigned char flags)
	: id(id), boundType(boundType), switches(flags), currentNoInstances(0), maxNoInstances(maxNoInstances) {
}

unsigned int Model::generateInstance(glm::vec3 size, float mass, glm::vec3 pos) {
	// all slots filled
	if (currentNoInstances >= maxNoInstances) {
		return -1;
	}

	instances.push_back(RigidBody(&id, size, mass, pos));
	return currentNoInstances++;

	// find first available slot
	/*for (unsigned int i = 0; i < maxNoInstances; i++) {
		if (!States::isIndexActive(&activeInstances, i)) {
			instances[i] = RigidBody(&id, size, mass, pos);
			currentNoInstances++;
			States::activateIndex(&activeInstances, i);
			return i;
		}
	}*/
}

void Model::initInstances() {
	/*
		if CONST_INSTANCES, instances already generated, set VBO with that data
	*/

	glm::vec3* posData = nullptr;
	glm::vec3* sizeData = nullptr;
	GLenum usage = GL_DYNAMIC_DRAW;

	if (States::isActive<unsigned char>(&switches, CONST_INSTANCES)) {
		// set data pointers accordingly

		for (int i = 0; i < currentNoInstances; i++) {
			positions.push_back(instances[i].pos);
			sizes.push_back(instances[i].size);
		}

		if (positions.size() > 0) {
			posData = &positions[0];
			sizeData = &sizes[0];
		}
		
		usage = GL_STATIC_DRAW;
	}

	// generate positions VBO
	posVBO = BufferObject(GL_ARRAY_BUFFER);
	posVBO.generate();
	posVBO.bind();
	posVBO.setData<glm::vec3>(maxNoInstances, posData, usage);

	// generate size VBO
	sizeVBO = BufferObject(GL_ARRAY_BUFFER);
	sizeVBO.generate();
	sizeVBO.bind();
	sizeVBO.setData<glm::vec3>(maxNoInstances, sizeData, usage);

	for (unsigned int i = 0, size = meshes.size(); i < size; i++) {
		meshes[i].VAO.bind();

		// set the vertex attribute pointers
		// positions
		posVBO.bind();
		posVBO.setAttPointer<glm::vec3>(3, 3, GL_FLOAT, 1, 0, 1);
		// size
		sizeVBO.bind();
		sizeVBO.setAttPointer<glm::vec3>(4, 3, GL_FLOAT, 1, 0, 1);

		ArrayObject::clear();
	}
}

void Model::removeInstance(unsigned int idx) {
	instances.erase(instances.begin() + idx);
	currentNoInstances--;
}

// get index of instance with id
unsigned int Model::getIdx(std::string id) {
	for (int i = 0; i < currentNoInstances; i++) {
		if (instances[i] == id) {
			return i;
		}
	}
	return -1;
}

void Model::render(Shader shader, float dt, Scene *scene, bool setModel) {
	if (setModel) {
		shader.setMat4("model", glm::mat4(1.0f));
	}

	if (!States::isActive<unsigned char>(&switches, CONST_INSTANCES)) {
		// update vbo data
		positions.clear();
		sizes.clear();

		bool doUpdate = States::isActive<unsigned char>(&switches, DYNAMIC);
		// update each instance

		for (int i = 0; i < currentNoInstances; i++) {
			if (doUpdate) {
				instances[i].update(dt);
			}
			positions.push_back(instances[i].pos);
			sizes.push_back(instances[i].size);
		}

		posVBO.bind();
		posVBO.updateData<glm::vec3>(0, currentNoInstances, &positions[0]);

		sizeVBO.bind();
		sizeVBO.updateData<glm::vec3>(0, currentNoInstances, &sizes[0]);
	}
	// otherwise, data is already set in VBOs

	shader.setFloat("material.shininess", 0.5f);

	for (unsigned int i = 0, noMeshes = meshes.size(); i < noMeshes; i++) {
		meshes[i].render(shader, currentNoInstances);
	}
}

void Model::cleanup() {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].cleanup();
	}
	posVBO.cleanup();
	sizeVBO.cleanup();
}

void Model::init() {}

void Model::loadModel(std::string path) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "Could not load 3d model at " << path << std::endl << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of("/"));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
	// process all nodes
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	// do same for children (recursive)
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	BoundingRegion br(boundType);

	// vars for AABB
	glm::vec3 min((float)(~0));		// initial set to largest integer
	glm::vec3 max(-(float)(~0));	// initial set to smallest integer

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		// process vertex

		// position
		vertex.pos = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		for (int j = 0; j < 3; j++) {
			// if smaller than min
			if (vertex.pos[j] < min[j]) min[j] = vertex.pos[j];
			// if larger than max
			if (vertex.pos[j] > max[j]) max[j] = vertex.pos[j];
		}

		// normal
		vertex.normal = glm::vec3(
			mesh->mNormals[i].x,
			mesh->mNormals[i].y,
			mesh->mNormals[i].z
		);

		// textures
		if (mesh->mTextureCoords[0]) {
			// ensure has tex coords
			vertex.texCoord = glm::vec2(
				mesh->mTextureCoords[0][i].x,
				mesh->mTextureCoords[0][i].y
			);
		}
		else {
			vertex.texCoord = glm::vec2(0.0f);
		}

		vertices.push_back(vertex);
	}

	if (boundType == BoundTypes::AABB) {
		// assign max and min
		br.min = min;
		br.max = max;
	}
	else {
		// calculate max distance from center
		br.center = BoundingRegion(min, max).calculateCenter();
		float maxRadiusSquared = 0.0f;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
			float radiusSquared = 0.0f;
			for (int j = 0; j < 3; j++) {
				radiusSquared += (vertices[i].pos[j] - br.center[j]) * (vertices[i].pos[j] - br.center[j]);
			}
			if (radiusSquared > maxRadiusSquared) {
				maxRadiusSquared = radiusSquared;
			}
		}

		br.radius = sqrt(maxRadiusSquared);
	}

	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh ret;

	// process material
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		if (States::isActive<unsigned char>(&switches, NO_TEX)) {
			// 1. diffuse colors
			aiColor4D diff(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);
			// 2. specular colors
			aiColor4D spec(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);

			ret = Mesh(br, diff, spec);
		}
		else {
			// 1. diffuse maps
			std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. specular maps
			std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			ret = Mesh(br, textures);
		}
	}

	ret.loadData(vertices, indices);
	return ret;
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type) {
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);

		// prevent duplicate loading
		bool skip = false;
		for (unsigned int j = 0; j < textures_loaded.size(); j++) {
			if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}

		if (!skip) {
			// not loaded yet
			Texture tex(directory, str.C_Str(), type);
			tex.load();
			textures.push_back(tex);
			textures_loaded.push_back(tex);
		}
		
	}

	return textures;
}