/*
 * Block.h
 *
 *  Created on: Aug 24, 2013
 *      Author: Tyler
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include "Renderer.h"
#include "Building.h"

#include <glm/glm.hpp>

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
	Block(int x, int z);
	virtual ~Block();

	void draw(glm::mat4 * viewMatrix);

	int * getLotArray();
	int getNumBuildings();
	int getBlockXCoord();
	int getBlockZCoord();

	static const int blockWidth = 3, blockDepth = 3;
	static const int lotScale = 40;//Why doesnt this work when lotScale = 1?
	static const float streetWidth = lotScale * ((blockWidth + blockDepth) / 2.f) * (1.f / 3);

	friend class Building;
private:
	Renderer streetRenderer;

	int numBuildings;
	Building * buildingArray;

	int blockX, blockZ;
	int * lotArray;

	int getLotIndex(int x, int y);
};

#endif /* BLOCK_H_ */
