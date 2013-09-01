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

	enum Priority {
		NO_CONNECTION,

		ONE_EDGE_NO_DIAG_SIDE,

		ONE_EDGE_NO_DIAG_CORNER,

		ONE_EDGE_NO_DIAG_CENTER,

		ONE_EDGE_ONE_DIAG_SIDE,

		ONE_EDGE_ONE_DIAG_CORNER,

		ONE_EDGE_ONE_DIAG_CENTER,

		TWO_EDGE_ONE_DIAG_SIDE,

		TWO_EDGE_ONE_DIAG_CORNER,

		TWO_EDGE_ONE_DIAG_CENTER,
	};

	enum Position {
		POS_DEFAULT,
		POS_SIDE,
		POS_CORNER,
		POS_CENTER
	};
	Block();
	virtual ~Block();

	void draw();

	int * getLotArray();
	int getNumBuildings();

	static const int blockWidth = 6, blockDepth = 6;
	static const int lotScale = 100;

	friend class Building;
private:
	int numBuildings;
	Building * buildingArray;

	int * lotArray;

	int getLotIndex(int x, int y);
};

#endif /* BLOCK_H_ */
