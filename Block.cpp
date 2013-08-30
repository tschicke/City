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

Block::Block() {
	Noise random(time(0));
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
		float buildingSize = random.nextFloat();
		buildingSize *= buildingSize;
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
		int startOffset = (random.nextInt() % (maxNumBuildings - 1)) + 1;
		int startX = 0, startZ = 0;
		int xDir = 1, zDir = 0;
		int i = 0;
		if (building != 0) {
			while(i < startOffset){
				if(startX + xDir > blockWidth - 1){
					xDir = 0;
					zDir = 1;
				}
				if(startZ + zDir > blockDepth - 1){
					xDir = -1;
					zDir = 0;
				}
				if(startX + xDir < 0){
					xDir = 0;
					zDir = -1;
				}
				if(startZ + zDir < 0){
					xDir = 1;
					zDir = 0;
				}

				startX += xDir;
				startZ += zDir;

				if(lotArray[getLotIndex(startX, startZ)] == -1){
					++i;
				}
			}
		}
		lotArray[getLotIndex(startX, startZ)] = building;

		std::cout << "\nLot array\n" << building + 1 << "\n";

		for (int x = 0; x < blockWidth; ++x) {
			for (int z = 0; z < blockDepth; ++z) {
				std::cout << lotArray[getLotIndex(x, z)] + 1 << ' ';
			}
			std::cout << '\n';
		}

		for (int buildingLot = 1; buildingLot < buildingSize; ++buildingLot){
			int lotX = -1, lotZ = -1;
			int priority = 0;

			for(int x1 = 0; x1 < blockWidth; ++x1){
				for(int z1 = 0; z1 < blockDepth; ++z1){
					Position position = POS_DEFAULT;
					Priority lotPriority = NO_CONNECTION;
					int numEdges = 0;
					int numDiags = 0;

					int lotX_Z = lotArray[getLotIndex(x1, z1)];
					if(lotX_Z != -1){
						continue;
					}

					int lotXM1_Z = lotArray[getLotIndex(x1 - 1, z1)];
					int lotXM1_ZP1 = lotArray[getLotIndex(x1 - 1, z1 + 1)];
					int lotX_ZP1 = lotArray[getLotIndex(x1, z1 + 1)];
					int lotXP1_ZP1 = lotArray[getLotIndex(x1 + 1, z1 + 1)];
					int lotXP1_Z = lotArray[getLotIndex(x1 + 1, z1)];
					int lotXP1_ZM1 = lotArray[getLotIndex(x1 + 1, z1 - 1)];
					int lotX_ZM1 = lotArray[getLotIndex(x1, z1 - 1)];
					int lotXM1_ZM1 = lotArray[getLotIndex(x1 - 1, z1 - 1)];

					//Check position

					if(((x1 == 0) || (x1 == blockWidth - 1)) && ((z1 == 0) || (z1 == blockDepth - 1))){//Corner
						position = POS_CORNER;
					} else if(x1 == 0 || x1 == blockWidth - 1 || z1 == 0 || z1 == blockDepth -1){//Side
						position = POS_SIDE;
					} else {//Center
						position = POS_CENTER;
					}

					//Check edges
					if(lotXM1_Z == building){
						++numEdges;
					}
					if(lotXP1_Z == building){
						++numEdges;
					}
					if(lotX_ZM1 == building){
						++numEdges;
					}
					if(lotX_ZP1 == building){
						++numEdges;
					}

					//Check diagonals
					if(lotXM1_ZM1 == building){
						++numDiags;
					}
					if(lotXP1_ZM1 == building){
						++numDiags;
					}
					if(lotXM1_ZP1 == building){
						++numDiags;
					}
					if(lotXP1_ZP1 == building){
						++numDiags;
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
						if(random.nextFloat() < 0.5f){
							lotX = x1;
							lotZ = z1;
							priority = lotPriority;
						}
					} else if(lotPriority > priority){
						lotX = x1;
						lotZ = z1;
						priority = lotPriority;
					}
				}
			}

			lotArray[getLotIndex(lotX, lotZ)] = building;

			std::cout << "\nLot array\n" << building + 1 << "\n";

			for (int x = 0; x < blockWidth; ++x) {
				for (int z = 0; z < blockDepth; ++z) {
					std::cout << lotArray[getLotIndex(x, z)] + 1 << ' ';
				}
				std::cout << '\n';
			}
		}
	}

	//Printing

	std::cout << numBuildings << '\n';

	for (int i = 0; i < numBuildings; ++i) {
		std::cout << "Building " << i + 1 << ": " << buildingSizes[i] << '\n';
	}

	/*std::cout << "\nLot array\n\n";

	for (int x = 0; x < blockWidth; ++x) {
		for (int z = 0; z < blockDepth; ++z) {
			std::cout << lotArray[getLotIndex(x, z)] + 1 << ' ';
		}
		std::cout << '\n';
	}*/
}

Block::~Block() {
}

void Block::draw() {
	for (int i = 0; i < numBuildings; ++i) {
		buildingArray[i].draw();
	}
}

int Block::getLotIndex(int x, int y) {
	return x * blockDepth + y;
}
