#include "model.h"
#include "../physics/environment.h"

#include <iostream>

Model::Model(BoundTypes boundType, glm::vec3 pos, glm::vec3 size, bool noTex, bool dynamic)
	: boundType(boundType), size(size), noTex(noTex), dynamic(dynamic) {
	rb.pos = pos;
 }

void Model::render(Shader shader, float dt, Box *b, bool setModel, bool doRender) {
	if (dynamic) {
		rb.update(dt);
	}

	if (setModel) {
 		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, rb.pos);
		model = glm::scale(model, size);

		shader.setMat4("model", model);
	}

	shader.setFloat("material.shininess", 0.5f);

	for (unsigned int i = 0, noMeshes = meshes.size(); i < noMeshes; i++) {
		meshes[i].render(shader, rb.pos, size, b, doRender);
	}
}

void Model::cleanup() {
	for (unsigned int i = 0; i < meshes.size(); i++) {
		meshes[i].cleanup();
	}
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

	// vars for sphere
	float radius = 0.0f;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		// process vertex

		// position
		vertex.pos = glm::vec3(
			mesh->mVertices[i].x,
			mesh->mVertices[i].y,
			mesh->mVertices[i].z
		);

		if (boundType == BoundTypes::AABB) {
			for (int i = 0; i < 3; i++) {
				// if smaller than min
				if (vertex.pos[i] < min[i]) min[i] = vertex.pos[i];
				// if larger than max
				if (vertex.pos[i] > max[i]) max[i] = vertex.pos[i];
			}
		}
		else {
			float dist = 0.0f;
			for (int i = 0; i < 3; i++) {
				dist += vertex.pos[i] * vertex.pos[i];
			}
			if (dist > radius) {
				// found new outer radius (squared)
				radius = dist;
			}
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
		br.min = min;
		br.max = max;
	}
	else {
		br.radius = sqrt(radius); // take square root at end
		br.center = glm::vec3(0.0f);
	}

	// process indices
	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	// process material
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		if (noTex) {
			// 1. diffuse colors
			aiColor4D diff(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);
			// 2. specular colors
			aiColor4D spec(1.0f);
			aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);

			return Mesh(br, vertices, indices, diff, spec);
		}
		else {
			// 1. diffuse maps
			std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE);
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. specular maps
			std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR);
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

			return Mesh(br, vertices, indices, textures);
		}
	}
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