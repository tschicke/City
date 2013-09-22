/*
 * Block.cpp
 *
 *  Created on: Aug 24, 2013
 *      Author: Tyler
 */

#include "Block.h"

#include <time.h>
#include <math.h>
#include <iostream>

#include "Noise.h"
#include "MathHelper.h"

Block::Block(){
	blockX = blockZ = 0;
	buildingArray = NULL;
	numBuildings = 0;
	lotArray = NULL;
}

Block::Block(int x, int z) {
	blockX = x;
	blockZ = z;

	streetRenderer.initShaders("shaders/colorShader.vert", "shaders/colorShader.frag");
	float floor[] = {
			getBlockXCoord() - (streetWidth / 2), 0, getBlockZCoord() - (streetWidth / 2),
			getBlockXCoord() + (blockWidth * lotScale) + (streetWidth / 2), 0, getBlockZCoord() - (streetWidth / 2),
			getBlockXCoord() + (blockWidth * lotScale) + (streetWidth / 2), 0, getBlockZCoord() + (blockWidth * lotScale) + (streetWidth / 2),
			getBlockXCoord() - (streetWidth / 2), 0, getBlockZCoord() + (blockWidth * lotScale) + (streetWidth / 2),

			0.35f, 0.35f, 0.35f,
			0.35f, 0.35f, 0.35f,
			0.35f, 0.35f, 0.35f,
			0.35f, 0.35f, 0.35f,

			0, 1, 0,
			0, 1, 0,
			0, 1, 0,
			0, 1, 0,
	};
	unsigned int floorIndices[] = {
			0, 1, 2,
			0, 2, 3
	};
	streetRenderer.allocBuffers(sizeof(floor), sizeof(floorIndices));
	streetRenderer.subVertexData(0, sizeof(floor), floor);
	streetRenderer.subIndexData(0, sizeof(floorIndices), floorIndices);
	streetRenderer.setNumVertices(4, 6);

	int seed = time(0) + (x * 65537) + (z * 4730861);
	Noise r(seed);
	std::cout << "seed = " << seed << '\n';
	int numLots = blockWidth * blockDepth;
	int maxNumBuildings = (blockWidth * 2) + (blockDepth * 2) - 4;
	lotArray = new int[numLots];
	numBuildings = 0;
	buildingArray = new Building[numBuildings];
	int buildingSizes[maxNumBuildings];

	int lotsTaken = 0;
	int lotIndex = 0;
	while (lotsTaken < numLots) {
		numBuildings++;
		float buildingSize = (r.nextFloat() + 1) / 2.f;
		buildingSize *= buildingSize * buildingSize * buildingSize;
		buildingSize = ceil(buildingSize * numLots);

		if (lotsTaken + buildingSize > numLots) {
			buildingSize = numLots - lotsTaken;
		}

		if (numBuildings == maxNumBuildings) {
			buildingSize = numLots - lotsTaken;
		}
		buildingSizes[lotIndex] = buildingSize;
		lotsTaken += buildingSize;
		lotIndex++;
	}

	MathHelper::reverseBubbleSort(buildingSizes, numBuildings);

	for(int i = 0; i < numLots; ++i){
		lotArray[i] = -1;
	}

	for(int building = 0; building < numBuildings; ++building){
		int buildingSize = buildingSizes[building];
		//Start position
//		int startOffset = (random.nextInt() % (maxNumBuildings - 1)) + 1;
		int startX = 0, startZ = 0;
		float rand1 = r.nextFloat();
		float rand2 = r.nextFloat();
		if(rand1 > 0.5f){
			startX = (blockWidth - 1) / 2;
			if(rand2 > 0.5f){
				startZ = 0;
			} else {
				startZ = blockDepth;
			}
		} else {
			startZ = (blockDepth - 1) / 2;
			if(rand2 > 0.5f){
				startX = 0;
			} else {
				startZ = (blockWidth - 1) / 2;
			}
		}
		int xDir = 1, zDir = 0;
		while (lotArray[getLotIndex(startX, startZ)] != -1) {
			if (startX + xDir > blockWidth - 1) {
				xDir = 0;
				zDir = 1;
			}
			if (startZ + zDir > blockDepth - 1) {
				xDir = -1;
				zDir = 0;
			}
			if (startX + xDir < 0) {
				xDir = 0;
				zDir = -1;
			}
			if (startZ + zDir < 0) {
				xDir = 1;
				zDir = 0;
			}

			startX += xDir;
			startZ += zDir;
		}
		lotArray[getLotIndex(startX, startZ)] = building;

		/*std::cout << "\nLot array\n" << building + 1 << "\n";

		for (int x = 0; x < blockWidth; ++x) {
			for (int z = 0; z < blockDepth; ++z) {
				std::cout << lotArray[getLotIndex(x, z)] + 1 << ' ';
			}
			std::cout << '\n';
		}*/

		for (int buildingLot = 1; buildingLot < buildingSize; ++buildingLot){
			int lotX = -1, lotZ = -1;
			int priority = 0;

			for(int x = 0; x < blockWidth; ++x){
				for(int z = 0; z < blockDepth; ++z){
					Position position = POS_DEFAULT;
					Priority lotPriority = NO_CONNECTION;
					int numEdges = 0;
					int numDiags = 0;

					int lotX_Z = lotArray[getLotIndex(x, z)];
					if(lotX_Z != -1){
						continue;
					}

					int lotXM1_Z = lotArray[getLotIndex(x - 1, z)];
					int lotXM1_ZP1 = lotArray[getLotIndex(x - 1, z + 1)];
					int lotX_ZP1 = lotArray[getLotIndex(x, z + 1)];
					int lotXP1_ZP1 = lotArray[getLotIndex(x + 1, z + 1)];
					int lotXP1_Z = lotArray[getLotIndex(x + 1, z)];
					int lotXP1_ZM1 = lotArray[getLotIndex(x + 1, z - 1)];
					int lotX_ZM1 = lotArray[getLotIndex(x, z - 1)];
					int lotXM1_ZM1 = lotArray[getLotIndex(x - 1, z - 1)];

					//Check position
					if(((x == 0) || (x == blockWidth - 1)) && ((z == 0) || (z == blockDepth - 1))){//Corner
						position = POS_CORNER;
					} else if(x == 0 || x == blockWidth - 1 || z == 0 || z == blockDepth - 1){//Side
						position = POS_SIDE;
					} else {//Center
						position = POS_CENTER;
					}

					//Check edges
					if(x != 0 && lotXM1_Z == building){
						++numEdges;
					}
					if(x != blockWidth - 1 && lotXP1_Z == building){
						++numEdges;
					}
					if(z != 0 && lotX_ZM1 == building){
						++numEdges;
					}
					if(z != blockDepth - 1 && lotX_ZP1 == building){
						++numEdges;
					}

					//Check diagonals
					if (x != 0) {
						if(z != 0 && lotXM1_ZM1 == building){
							++numDiags;
						}
						if(z != blockDepth - 1 && lotXM1_ZP1 == building){
							++numDiags;
						}
					}
					if (x != blockWidth - 1) {
						if(z != 0 && lotXP1_ZM1 == building){
							++numDiags;
						}
						if(z != blockDepth - 1 && lotXP1_ZP1 == building){
							++numDiags;
						}
					}

					if(position == POS_CENTER){
						if(numEdges == 0){
							continue;//Check
						} else if(numEdges == 1){
							if(numDiags == 0){
								lotPriority = ONE_EDGE_NO_DIAG_CENTER;
							} else if(numDiags == 1){
								lotPriority = ONE_EDGE_ONE_DIAG_CENTER;
							}
						} else if(numEdges == 2){
							lotPriority = TWO_EDGE_ONE_DIAG_CENTER;
						}
					} else if(position == POS_CORNER){
						if(numEdges == 0){
							continue;//Check
						} else if(numEdges == 1){
							if(numDiags == 0){
								lotPriority = ONE_EDGE_NO_DIAG_CORNER;
							} else if(numDiags == 1){
								lotPriority = ONE_EDGE_ONE_DIAG_CORNER;
							}
						} else if(numEdges == 2){
							lotPriority = TWO_EDGE_ONE_DIAG_CORNER;
						}
					} else if(position == POS_SIDE){
						if(numEdges == 0){
							continue;//Check
						} else if(numEdges == 1){
							if(numDiags == 0){
								lotPriority = ONE_EDGE_NO_DIAG_SIDE;
							} else if(numDiags == 1){
								lotPriority = ONE_EDGE_ONE_DIAG_SIDE;
							}
						} else if(numEdges == 2){
							lotPriority = TWO_EDGE_ONE_DIAG_SIDE;
						}
					}

					if(lotPriority == priority){
						if(r.nextFloat() < 0.5f){
							lotX = x;
							lotZ = z;
						}
					} else if(lotPriority > priority){
						lotX = x;
						lotZ = z;
						priority = lotPriority;
					}
				}
			}

			lotArray[getLotIndex(lotX, lotZ)] = building;

			/*std::cout << "\nLot array\n" << building + 1 << "\n";

			for (int x = 0; x < blockWidth; ++x) {
				for (int z = 0; z < blockDepth; ++z) {
					std::cout << lotArray[getLotIndex(x, z)] + 1 << ' ';
				}
				std::cout << '\n';
			}*/
		}
	}

	buildingArray = new Building[numBuildings];
	for(int i = 0; i < numBuildings; ++i){
		buildingArray[i] = Building(buildingSizes[i], i, this);
	}

	//Printing

	std::cout << numBuildings << '\n';

	for (int i = 0; i < numBuildings; ++i) {
		std::cout << "Building " << i + 1 << ": " << buildingSizes[i] << '\n';
	}

	std::cout << "\nLot array\n\n";

	for (int z = blockDepth - 1; z > -1; --z) {
		for (int x = 0; x < blockWidth; ++x) {
			std::cout << lotArray[getLotIndex(x, z)] + 1 << ' ';
		}
		std::cout << '\n';
	}
}

Block::~Block() {
}

void Block::draw(glm::mat4 * viewMatrix) {
	for (int i = 0; i < numBuildings; ++i) {
		buildingArray[i].draw(viewMatrix);
	}
	streetRenderer.render(viewMatrix);
}

int* Block::getLotArray() {
	return lotArray;
}

int Block::getNumBuildings() {
	return numBuildings;
}

int Block::getBlockXCoord() {
	return (blockX * lotScale * blockWidth) + (blockX * streetWidth);
}

int Block::getBlockZCoord() {
	//Temp StreetWidth
	float streetWidth = lotScale * ((blockWidth + blockDepth) / 2.f) * (1.f / 3);
	return (blockZ * lotScale * blockDepth) + (blockZ * streetWidth);
}

int Block::getLotIndex(int x, int y) {
	return x * blockDepth + y;
}
