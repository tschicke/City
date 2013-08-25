/*
 * Block.h
 *
 *  Created on: Aug 24, 2013
 *      Author: Tyler
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include "Building.h"

class Block {
public:
	Block();
	virtual ~Block();

	void draw();

	static const int blockWidth = 3, blockDepth = 3;
private:
	int numBuildings;
	Building * buildingArray;

	int * lotArray;
};

#endif /* BLOCK_H_ */
