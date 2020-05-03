#pragma once

#include <GL/gl3w.h>
#include <freeglut 3.0.0/include/GL/freeglut.h>
#include <glm/glm/glm.hpp>
#include <vector>
#include <map>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "VAOData.h"

class VAOLoader
{
public:

	//Load from .obj file
	static void LoadOBJ(const std::string&);
	static void processNode(std::vector<float>&, std::vector<float>&, std::vector<float>&, std::vector<GLuint>&, aiNode*, const aiScene*);

	//Generic object VAO
	static VAOData* loadToVAO(std::vector<float> positions, std::vector<float> normals, std::vector<GLuint> indices, std::vector<float> texCoords);

	//Terrain Chunk VAO, since it needs a score per vertex.
	static VAOData* loadToVAO(std::vector<float> positions, std::vector<float> normals, std::vector<GLuint> indices, std::vector<float> texCoords, std::vector<float> scores);

	//Re-write a certain VBO of a VAO with this new data.
	static void updateVBOInVAO(GLuint vaoID, GLuint vboID, int attributeNumber, int numOfComponents, std::vector<float> data);

	static VAOLoader * getInstance();

	static VAOData* getModelVAO(const std::string& name) {
		if (models.count(name) != 0) return models[name];
	}

private:

	static VAOLoader * thisPointer;

	static std::map<std::string, VAOData*> models;

	VAOLoader();
	
	static std::vector<GLuint> vaos;
	static std::vector<GLuint> vbos;

	//Method to generically store a vector of floats in a vbo at attribute number, with a certain number of components per vertex.
	static GLuint storeFloatDataInAttributeList(int attributeNumber, int numOfComponents, std::vector<float> data);

	//Stores GLuint vector data in an element array buffer.
	static void storeIndicesDataInAttributeList(int attributeNumber, std::vector<GLuint> data);


	static int createVAO();
	
	static void unbindVAO();
	 
	static void cleanUp();
};

