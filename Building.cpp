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
	height = (randNum * Block::lotScale * size) + (Block::lotScale / 2.f);
	r = randNum + 0.1;
	g = randNum + 0.1;
	b = randNum + 0.1;

	buildBuilding();
}

Building::~Building() {
}

void Building::buildBuilding() {
	int * lotArray = parentBlock->getLotArray();
	int lotScale = Block::lotScale;

	int numVerticesPerLot = 4 * 6;//Should be 4 * 6
	int numVertices = numVerticesPerLot * buildingSize;

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

			float x1 = x * lotScale + (parentBlock->getBlockXCoord()), x2 = (x + 1) * lotScale + parentBlock->getBlockXCoord(), z1 = z * lotScale + parentBlock->getBlockZCoord(), z2 = (z + 1) * lotScale + parentBlock->getBlockZCoord();

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

					glm::vec3(x1, height, z1),//Top
					glm::vec3(x2, height, z1),
					glm::vec3(x2, height, z2),
					glm::vec3(x1, height, z2),

					glm::vec3(x1, 0, z1),//Left
					glm::vec3(x1, 0, z2),
					glm::vec3(x1, height, z2),
					glm::vec3(x1, height, z1),

					glm::vec3(x2, 0, z1),//Right
					glm::vec3(x2, 0, z2),
					glm::vec3(x2, height, z2),
					glm::vec3(x2, height, z1),

					glm::vec3(x1, 0, z1),//Back
					glm::vec3(x2, 0, z1),
					glm::vec3(x2, height, z1),
					glm::vec3(x1, height, z1),

					glm::vec3(x1, 0, z2),//Front
					glm::vec3(x2, 0, z2),
					glm::vec3(x2, height, z2),
					glm::vec3(x1, height, z2),
			};

			glm::vec3 tempNormalArray[] = {
					glm::vec3(0, 1, 0),//Bottom
					glm::vec3(0, 1, 0),
					glm::vec3(0, 1, 0),
					glm::vec3(0, 1, 0),

					glm::vec3(0, -1, 0),//Top
					glm::vec3(0, -1, 0),
					glm::vec3(0, -1, 0),
					glm::vec3(0, -1, 0),

					glm::vec3(1, 0, 0),//Left
					glm::vec3(1, 0, 0),
					glm::vec3(1, 0, 0),
					glm::vec3(1, 0, 0),

					glm::vec3(-1, 0, 0),//Right
					glm::vec3(-1, 0, 0),
					glm::vec3(-1, 0, 0),
					glm::vec3(-1, 0, 0),

					glm::vec3(0, 0, -1),//Back
					glm::vec3(0, 0, -1),
					glm::vec3(0, 0, -1),
					glm::vec3(0, 0, -1),

					glm::vec3(0, 0, 1),//Front
					glm::vec3(0, 0, 1),
					glm::vec3(0, 0, 1),
					glm::vec3(0, 0, 1),
			};

			unsigned int tempIndices[] = {
					0, 1, 2,//Bottom
					0, 2, 3,

					4, 5, 6,//Top
					4, 6, 7,

					8, 9, 10,//Left
					8, 10, 11,

					12, 13, 14,//Right
					12, 14, 15,

					16, 17, 18,//Back
					16, 18, 19,

					20, 21, 22,//Front
					20, 22, 23,
			};

			int dataIndex = sizeCounter * numVerticesPerLot;
			for(int i = 0; i < numVerticesPerLot; ++i){
				vertexData[dataIndex + i] = tempVertexArray[i];
				colorData[dataIndex + i] = glm::vec3(r, g, b);
				normals[dataIndex + i] = tempNormalArray[i];//Temp
			}

			//Bottom and Top
			for (int i = 0; i < 12; ++i) {
				indices.push_back(tempIndices[i] + dataIndex);
			}

			//Left
			if (x != 0) {
				if (lotArray[parentBlock->getLotIndex(x - 1, z)] != buildingID) {
					for (int i = 12; i < 18; ++i) {
						indices.push_back(tempIndices[i] + dataIndex);
					}
				}
			} else {
				for (int i = 12; i < 18; ++i) {
					indices.push_back(tempIndices[i] + dataIndex);
				}
			}

			//Right
			if (x != Block::blockWidth - 1) {
				if (lotArray[parentBlock->getLotIndex(x + 1, z)] != buildingID) {
					for (int i = 18; i < 24; ++i) {
						indices.push_back(tempIndices[i] + dataIndex);
					}
				}
			} else {
				for (int i = 18; i < 24; ++i) {
					indices.push_back(tempIndices[i] + dataIndex);
				}
			}

			//Back
			if (z != 0) {
				if (lotArray[parentBlock->getLotIndex(x, z - 1)] != buildingID) {
					for (int i = 24; i < 30; ++i) {
						indices.push_back(tempIndices[i] + dataIndex);
					}
				}
			} else {
				for (int i = 24; i < 30; ++i) {
					indices.push_back(tempIndices[i] + dataIndex);
				}
			}

			//Front
			if (z != Block::blockDepth - 1) {
				if (lotArray[parentBlock->getLotIndex(x, z + 1)] != buildingID) {
					for (int i = 30; i < 36; ++i) {
						indices.push_back(tempIndices[i] + dataIndex);
					}
				}
			} else {
				for (int i = 30; i < 36; ++i) {
					indices.push_back(tempIndices[i] + dataIndex);
				}
			}

			++sizeCounter;
		}
	}

	buildingRenderer.initShaders("shaders/colorShader.vert", "shaders/colorShader.frag");

	buildingRenderer.allocBuffers(numVertices * 3 * 3 * sizeof(float), indices.size() * sizeof(unsigned int));
	buildingRenderer.subVertexData(0, numVertices * 3 * sizeof(float), (float *)vertexData);
	buildingRenderer.subVertexData(numVertices * 3 * sizeof(float), numVertices * 3 * sizeof(float), (float *)colorData);
	buildingRenderer.subVertexData(numVertices * 3 * sizeof(float) * 2, numVertices * 3 * sizeof(float), (float *)normals);
	buildingRenderer.subIndexData(0, indices.size() * sizeof(unsigned int), indices.data());

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
