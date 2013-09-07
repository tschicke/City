/*
 * Building.h
 *
 *  Created on: Aug 23, 2013
 *      Author: Tyler
 */

#ifndef BUILDING_H_
#define BUILDING_H_

#include "Common.h"

#include "Renderer.h"

class Block;

class Building {
public:
	Building();
	Building(int size, int buildingID, Block * parentBlock);
	virtual ~Building();

	void draw();

private:
	void buildBuilding();

	int buildingID;

	int height;
	int size;

	Block * parentBlock;

	Renderer buildingRenderer;

	//Temp
	float r, g, b;
};

#endif /* BUILDING_H_ */
