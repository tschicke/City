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

Building::Building() {
	size = 0;
	buildingID = -1;
	parentBlock = NULL;
	height = 0;
	r = 1;
	g = 1;
	b = 1;
}

Building::Building(int size, int buildingID, Block* parentBlock) {
	this->size = size;
	this->buildingID = buildingID;
	this->parentBlock = parentBlock;
	Noise random(time(0) * ((buildingID + 1) * (buildingID + 97)));
	float randNum = (random.nextFloat() + 1) / 2.f;
	height = (randNum * Block::lotScale * size) + (Block::lotScale / 2);
	r = randNum;
	g = randNum;
	b = randNum;
}

Building::~Building() {
}

void Building::draw() {
	int * lotArray = parentBlock->getLotArray();
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
	glEnd();
}
