/*
 * Building.h
 *
 *  Created on: Aug 23, 2013
 *      Author: Tyler
 */

#ifndef BUILDING_H_
#define BUILDING_H_

#include "Common.h"

class Block;

class Building {
public:
	Building();
	Building(int size, int buildingID, Block * parentBlock);
	virtual ~Building();

	void draw();

private:
	int buildingID;

	int height;
	int size;

	Block * parentBlock;

	//Temp
	float r, g, b;
};

#endif /* BUILDING_H_ */
