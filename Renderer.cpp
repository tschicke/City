/*
 * Renderer.cpp
 *
 *  Created on: Sep 4, 2013
 *      Author: Tyler
 */

#include "Renderer.h"

#include "Common.h"

#include <gl/glew.h>

#include <glm/gtx/transform.hpp>

glm::mat4 Renderer::projectionMatrix;
int Renderer::fov = 0, Renderer::width = 0, Renderer::height = 0, Renderer::renderDistance = 0;

void Renderer::setProjectionMatrix(int fov, int width, int height, int renderDistance) {
	Renderer::fov = fov;
	Renderer::width = width;
	Renderer::height = height;
	Renderer::renderDistance = renderDistance;
	projectionMatrix = glm::perspective((float)fov, (float)width / (float) height, 0.5f, (float)renderDistance);
}

void Renderer::setFov(int fov) {
	Renderer::fov = fov;
	projectionMatrix = glm::perspective((float)fov, (float)width / (float) height, 0.5f, (float)renderDistance);
}

void Renderer::setProjectionSize(int width, int height) {
	Renderer::width = width;
	Renderer::height = height;
	projectionMatrix = glm::perspective((float)fov, (float)width / (float) height, 0.5f, (float)renderDistance);
}

void Renderer::setRenderDistance(int renderDistance) {
	Renderer::renderDistance = renderDistance;
	projectionMatrix = glm::perspective((float)fov, (float)width / (float) height, 0.5f, (float)renderDistance);
}

Renderer::Renderer() {
	vertexBufferID = -1;
	indexBufferID = -1;
	buffersAllocated = false;
	shadersInitialized = false;
	numVertices = 0;
	numVerticesToDraw = 0;
}

Renderer::~Renderer() {
}

//Renderer functions
void Renderer::allocBuffers(int vertexBufferSize, int indexBufferSize) {
	if (buffersAllocated) {
		return;
	}

	glGenBuffers(1, &vertexBufferID);
	glGenBuffers(1, &indexBufferID);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	buffersAllocated = true;
}

void Renderer::deleteBuffers() {
	if (!buffersAllocated) {
		return;
	}

	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &indexBufferID);

	vertexBufferID = -1;
	indexBufferID = -1;
}

void Renderer::subVertexData(int offset, int size, int* data) {
	if (!buffersAllocated) {
		return;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::subVertexData(int offset, int size, float* data) {
	if (!buffersAllocated) {
		return;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::subIndexData(int offset, int size, unsigned int* data) {
	if (!buffersAllocated) {
		return;
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vertexBufferID);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::initShaders(const char* vertexShaderPath, const char* fragmentShaderPath) {
	Shader vertexShader;
	vertexShader.loadShader(vertexShaderPath, GL_VERTEX_SHADER);

	Shader fragmentShader;
	fragmentShader.loadShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	shaderProgram.createProgram();
	shaderProgram.addShader(&vertexShader);
	shaderProgram.addShader(&fragmentShader);
	shaderProgram.linkProgram();

	vertexShader.deleteShader();
	fragmentShader.deleteShader();
}

void Renderer::render(glm::mat4* viewMatrix) {
	shaderProgram.useProgram();

	shaderProgram.setUniform("modelMatrix", &modelMatrix, 1);
	shaderProgram.setUniform("viewMatrix", viewMatrix, 1);
	shaderProgram.setUniform("projectionMatrix", &projectionMatrix, 1);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) (sizeof(float) * numVertices * 3));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void *) (sizeof(float) * numVertices * 3));

	glDrawElements(GL_TRIANGLES, numVerticesToDraw, GL_UNSIGNED_INT, (void *) 0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glUseProgram(0);
}

//Transformation functions
void Renderer::loadIdentityMatrix(){
	modelMatrix = glm::mat4();
}

void Renderer::translate(glm::vec3 translateVector) {
	modelMatrix = glm::translate(modelMatrix, translateVector);//Which version of funtions to use?
}

void Renderer::translate(float x, float y, float z) {
	modelMatrix = glm::translate(modelMatrix, x, y, z);
}

void Renderer::rotate(float angle, glm::vec3 rotateVector) {
	modelMatrix = glm::rotate(modelMatrix, angle, rotateVector);
}

void Renderer::rotate(float angle, float x, float y, float z) {
	modelMatrix = glm::rotate(modelMatrix, angle, x, y, z);
}

void Renderer::scale(glm::vec3 scaleVector) {
	modelMatrix = glm::scale(modelMatrix, scaleVector);
}

void Renderer::scale(float x, float y, float z) {
	modelMatrix = glm::scale(modelMatrix, x, y, z);
}
