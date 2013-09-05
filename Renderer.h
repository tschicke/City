/*
 * Renderer.h
 *
 *  Created on: Sep 4, 2013
 *      Author: Tyler
 */

#ifndef RENDERER_H_
#define RENDERER_H_

#include <glm/glm.hpp>

#include "Shader.h"

class Renderer {
public:
	Renderer();
	virtual ~Renderer();

	static void setProjectionMatrix(int fov, int width, int height, int renderDistance);
	static void setFov(int fov);
	static void setProjectionSize(int width, int height);
	static void setRenderDistance(int renderDistance);

	//Renderer functions
	void allocBuffers(int vertexBufferSize, int indexBufferSize);
	void deleteBuffers();

	void subVertexData(int offset, int size, int * data);
	void subVertexData(int offset, int size, float * data);
	void subIndexData(int offset, int size, unsigned int * data);

	void initShaders(const char * vertexShaderPath, const char * fragmentShaderPath);

	void render(glm::mat4 * viewMatrix);

	//Transformation Functions
	void loadIdentityMatrix();

	void translate(glm::vec3 translateVector);
	void translate(float x, float y, float z);

	void rotate(float angle, glm::vec3 rotateVector);
	void rotate(float angle, float x, float y, float z);

	void scale(glm::vec3 scaleVector);
	void scale(float x, float y, float z);

private:
	static glm::mat4 projectionMatrix;
	static int fov, width, height, renderDistance;

	unsigned int vertexBufferID, indexBufferID;
	bool buffersAllocated;

	ShaderProgram shaderProgram;
	bool shadersInitialized;

	glm::mat4 modelMatrix;

	int numVertices, numVerticesToDraw;

};

#endif /* RENDERER_H_ */
