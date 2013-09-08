/*
 * Building.cpp
 *
 *  Created on: Aug 23, 2013
 *      Author: Tyler
 */

#include "Building.h"

#include <iostream>

#include "Block.h"

#include <time.h>
#include "Noise.h"

#include <gl/glew.h>

#include <vector>

Building::Building() {
	buildingSize = 0;
	buildingID = -1;
	parentBlock = NULL;
	height = 0;
	r = 1;
	g = 1;
	b = 1;
}

Building::Building(int size, int buildingID, Block* parentBlock) {
	this->buildingSize = size;
	this->buildingID = buildingID;
	this->parentBlock = parentBlock;
	Noise random(time(0) * ((buildingID + 1) * (buildingID + 97)));
	float randNum = (random.nextFloat() + 1) / 2.f;
	height = (randNum * Block::lotScale * size) + (Block::lotScale / 2);
	r = randNum;
	g = randNum;
	b = randNum;

	buildBuilding();
}

Building::~Building() {
}

void Building::buildBuilding() {
	int * lotArray = parentBlock->getLotArray();
	int lotScale = Block::lotScale;

	int numVerticesPerLot = 4 * 1;//Should be 4 * 6
	int numVertices = numVerticesPerLot * buildingSize;
	int numIndicesPerLot = 6 * 1;//Should be 6 * 6

	float buildingPadding = lotScale / 20.f;

	glm::vec3 vertexData[numVertices];
	glm::vec3 colorData[numVertices];
	glm::vec3 normals[numVertices];

	std::vector<unsigned int> indices;

	int sizeCounter = 0;

	for(int x = 0; x < Block::blockWidth; ++x){
		for(int z = 0; z < Block::blockDepth; ++z){
			if(lotArray[parentBlock->getLotIndex(x, z)] != buildingID){
				continue;
			}

			int x1 = x * lotScale, x2 = (x + 1) * lotScale, z1 = z * lotScale, z2 = (z + 1) * lotScale;

			if (x != 0) {
				if (lotArray[parentBlock->getLotIndex(x - 1, z)] != buildingID) {
					x1 += buildingPadding;
					if (z != 0 && lotArray[parentBlock->getLotIndex(x - 1, z - 1)] == buildingID) {
						z1 -= buildingPadding;
					}
					if (z != Block::blockDepth - 1 && lotArray[parentBlock->getLotIndex(x - 1, z + 1)] == buildingID) {
						z2 += buildingPadding;
					}
				}

			} else {
				x1 += buildingPadding;
			}

			if (x != Block::blockWidth - 1) {
				if (lotArray[parentBlock->getLotIndex(x + 1, z)] != buildingID) {
					x2 -= buildingPadding;
					if (z != 0 && lotArray[parentBlock->getLotIndex(x + 1, z - 1)] == buildingID) {
						z1 -= buildingPadding;
					}
					if (z != Block::blockDepth - 1 && lotArray[parentBlock->getLotIndex(x + 1, z + 1)] == buildingID) {
						z2 += buildingPadding;
					}
				}
			} else {
				x2 -= buildingPadding;
			}

			if (z != 0) {
				if (lotArray[parentBlock->getLotIndex(x, z - 1)] != buildingID) {
					z1 += buildingPadding;
					if (x != 0 && lotArray[parentBlock->getLotIndex(x - 1, z - 1)] == buildingID) {
						x1 -= buildingPadding;
					}
					if (x != Block::blockWidth - 1 && lotArray[parentBlock->getLotIndex(x + 1, z - 1)] == buildingID) {
						x2 += buildingPadding;
					}
				}
			} else {
				z1 += buildingPadding;
			}

			if (z != Block::blockDepth - 1) {
				if (lotArray[parentBlock->getLotIndex(x, z + 1)] != buildingID) {
					z2 -= buildingPadding;
					if (x != 0 && lotArray[parentBlock->getLotIndex(x - 1, z + 1)] == buildingID) {
						x1 -= buildingPadding;
					}
					if (x != Block::blockWidth - 1 && lotArray[parentBlock->getLotIndex(x + 1, z + 1)] == buildingID) {
						x2 += buildingPadding;
					}
				}
			} else {
				z2 -= buildingPadding;
			}

			if (x != 0) {
				bool leftLot = lotArray[parentBlock->getLotIndex(x - 1, z)] == buildingID;
				if (z != 0) {
					bool downLot = lotArray[parentBlock->getLotIndex(x, z - 1)] == buildingID;
					if (leftLot && downLot && (lotArray[parentBlock->getLotIndex(x - 1, z - 1)] != buildingID)) {
						x1 += buildingPadding;
						z1 += buildingPadding;
					}
				}
				if (z != Block::blockDepth - 1) {
					bool upLot = lotArray[parentBlock->getLotIndex(x, z + 1)] == buildingID;
					if (leftLot && upLot && (lotArray[parentBlock->getLotIndex(x - 1, z + 1)] != buildingID)) {
						x1 += buildingPadding;
						z2 -= buildingPadding;
					}
				}
			}
			if (x != Block::blockWidth - 1) {
				bool rightLot = lotArray[parentBlock->getLotIndex(x + 1, z)] == buildingID;
				if (z != 0) {
					bool downLot = lotArray[parentBlock->getLotIndex(x, z - 1)] == buildingID;
					if (rightLot && downLot && (lotArray[parentBlock->getLotIndex(x + 1, z - 1)] != buildingID)) {
						x2 -= buildingPadding;
						z1 += buildingPadding;
					}
				}
				if (z != Block::blockDepth - 1) {
					bool upLot = lotArray[parentBlock->getLotIndex(x, z + 1)] == buildingID;
					if (rightLot && upLot && (lotArray[parentBlock->getLotIndex(x + 1, z + 1)] != buildingID)) {
						x2 -= buildingPadding;
						z2 -= buildingPadding;
					}
				}
			}

			glm::vec3 tempVertexArray[] = {
					glm::vec3(x1, 0, z1),//Bottom
					glm::vec3(x2, 0, z1),
					glm::vec3(x2, 0, z2),
					glm::vec3(x1, 0, z2),
			};

			unsigned int tempIndices[] = {
					0, 1, 2,
					0, 2, 3,
			};

			int dataIndex = sizeCounter * numVerticesPerLot;
			for(int i = 0; i < numVerticesPerLot; ++i){
				vertexData[dataIndex + i] = tempVertexArray[i];
				colorData[dataIndex + i] = glm::vec3(r, g, b);
				normals[dataIndex + i] = glm::vec3(0, 1, 0);//Temp
			}

			for(int i = 0; i < numIndicesPerLot; ++i){
				indices.push_back(tempIndices[i] + dataIndex);
			}



			++sizeCounter;
		}
	}

//	for(int i = 0; i < numVertices; ++i){
//		std::cout << vertexData[i].r << ' ' << vertexData[i].g << ' ' << vertexData[i].b << '\n';
//	}

//	for(unsigned int i = 0; i < indices.size(); ++i){
//		std::cout << indices[i] << '\n';
//	}

	buildingRenderer.initShaders("shaders/colorShader.vert", "shaders/colorShader.frag");

	buildingRenderer.allocBuffers(numVertices * 3 * 3 * sizeof(float), indices.size() * sizeof(unsigned int));
	buildingRenderer.subVertexData(0, numVertices * 3 * sizeof(float), (float *)vertexData);
	buildingRenderer.subVertexData(numVertices * 3 * sizeof(float), numVertices * 3 * sizeof(float), (float *)colorData);
	buildingRenderer.subVertexData(numVertices * 3 * sizeof(float) * 2, numVertices * 3 * sizeof(float), (float *)normals);
	buildingRenderer.subIndexData(0, indices.size() * sizeof(unsigned int), indices.data());

	std::cout << "numVerts " << indices.size() << '\n';

	buildingRenderer.setNumVertices(numVertices, indices.size());
}

void Building::draw(glm::mat4 * viewMatrix) {
/*	int * lotArray = parentBlock->getLotArray();
	int lotScale = Block::lotScale;
	glBegin(GL_LINES);
	for (int x = 0; x < Block::blockWidth; ++x) {
		for (int z = 0; z < Block::blockDepth; ++z) {
			if (lotArray[parentBlock->getLotIndex(x, z)] == buildingID) {
				int x1 = x * lotScale, x2 = (x + 1) * lotScale, z1 = z * lotScale, z2 = (z + 1) * lotScale;
				float buildingPadding = Block::lotScale / 20.f;

				if (x != 0) {
					if (lotArray[parentBlock->getLotIndex(x - 1, z)] != buildingID) {
						x1 += buildingPadding;
						if (z != 0 && lotArray[parentBlock->getLotIndex(x - 1, z - 1)] == buildingID) {
							z1 -= buildingPadding;
						}
						if (z != Block::blockDepth - 1 && lotArray[parentBlock->getLotIndex(x - 1, z + 1)] == buildingID) {
							z2 += buildingPadding;
						}
					}

				} else {
					x1 += buildingPadding;
				}

				if (x != Block::blockWidth - 1) {
					if (lotArray[parentBlock->getLotIndex(x + 1, z)] != buildingID) {
						x2 -= buildingPadding;
						if (z != 0 && lotArray[parentBlock->getLotIndex(x + 1, z - 1)] == buildingID) {
							z1 -= buildingPadding;
						}
						if (z != Block::blockDepth - 1 && lotArray[parentBlock->getLotIndex(x + 1, z + 1)] == buildingID) {
							z2 += buildingPadding;
						}
					}
				} else {
					x2 -= buildingPadding;
				}

				if (z != 0) {
					if (lotArray[parentBlock->getLotIndex(x, z - 1)] != buildingID) {
						z1 += buildingPadding;
						if (x != 0 && lotArray[parentBlock->getLotIndex(x - 1, z - 1)] == buildingID) {
							x1 -= buildingPadding;
						}
						if (x != Block::blockWidth - 1 && lotArray[parentBlock->getLotIndex(x + 1, z - 1)] == buildingID) {
							x2 += buildingPadding;
						}
					}
				} else {
					z1 += buildingPadding;
				}

				if (z != Block::blockDepth - 1) {
					if (lotArray[parentBlock->getLotIndex(x, z + 1)] != buildingID) {
						z2 -= buildingPadding;
						if (x != 0 && lotArray[parentBlock->getLotIndex(x - 1, z + 1)] == buildingID) {
							x1 -= buildingPadding;
						}
						if (x != Block::blockWidth - 1 && lotArray[parentBlock->getLotIndex(x + 1, z + 1)] == buildingID) {
							x2 += buildingPadding;
						}
					}
				} else {
					z2 -= buildingPadding;
				}



				if (x != 0) {
					bool leftLot = lotArray[parentBlock->getLotIndex(x - 1, z)] == buildingID;
					if (z != 0) {
						bool downLot = lotArray[parentBlock->getLotIndex(x, z - 1)] == buildingID;
						if (leftLot && downLot && (lotArray[parentBlock->getLotIndex(x - 1, z - 1)] != buildingID)) {
							x1 += buildingPadding;
							z1 += buildingPadding;
						}
					}
					if (z != Block::blockDepth - 1) {
						bool upLot = lotArray[parentBlock->getLotIndex(x, z + 1)] == buildingID;
						if (leftLot && upLot && (lotArray[parentBlock->getLotIndex(x - 1, z + 1)] != buildingID)) {
							x1 += buildingPadding;
							z2 -= buildingPadding;
						}
					}
				}
				if (x != Block::blockWidth - 1) {
					bool rightLot = lotArray[parentBlock->getLotIndex(x + 1, z)] == buildingID;
					if (z != 0) {
						bool downLot = lotArray[parentBlock->getLotIndex(x, z - 1)] == buildingID;
						if (rightLot && downLot && (lotArray[parentBlock->getLotIndex(x + 1, z - 1)] != buildingID)) {
							x2 -= buildingPadding;
							z1 += buildingPadding;
						}
					}
					if (z != Block::blockDepth - 1) {
						bool upLot = lotArray[parentBlock->getLotIndex(x, z + 1)] == buildingID;
						if (rightLot && upLot && (lotArray[parentBlock->getLotIndex(x + 1, z + 1)] != buildingID)) {
							x2 -= buildingPadding;
							z2 -= buildingPadding;
						}
					}
				}

				glColor3f(r, g, b);
				glVertex2f(x1, z1);
				glVertex2f(x2, z1);

				glVertex2f(x2, z1);
				glVertex2f(x2, z2);

				glVertex2f(x2, z2);
				glVertex2f(x1, z2);

				glVertex2f(x1, z2);
				glVertex2f(x1, z1);

			}
		}
	}
	glEnd();*/

	buildingRenderer.render(viewMatrix);
}
