/*
 * Block.cpp
 *
 *  Created on: Aug 24, 2013
 *      Author: Tyler
 */

#include "Block.h"

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <iostream>

#include "MathHelper.h"

Block::Block() {
	srand(time(0));
	for (int i = 0; i < 10; ++i) {
//		std::cout << rand() << '\n';
	}
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
		float buildingSize = (float) rand() / (float) RAND_MAX;
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

	for(int i = 0; i < numBuildings; ++i){

	}

	std::cout << numBuildings << '\n';

	for (int i = 0; i < numBuildings; ++i) {
		std::cout << "Building " << i + 1 << ": " << buildingSizes[i] << '\n';
	}

	std::cout << "\nLot array\n\n";

	for (int x = 0; x < blockWidth; ++x) {
		for (int z = 0; z < blockDepth; ++z) {
			std::cout << lotArray[x * blockDepth + z] << ' ';
		}
		std::cout << '\n';
	}
}

Block::~Block() {
}

void Block::draw() {
	for (int i = 0; i < numBuildings; ++i) {
		buildingArray[i].draw();
	}
}
