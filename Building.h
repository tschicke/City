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

#include <glm/glm.hpp>

class Block;

class Building {
public:
	Building();
	Building(int size, int buildingID, Block * parentBlock);
	virtual ~Building();

	void draw(glm::mat4 * viewMatrix);

private:
	void buildBuilding();

	int buildingID;

	int height;
	int buildingSize;

	Block * parentBlock;

	Renderer buildingRenderer;

	//Temp
	float r, g, b;
};

#endif /* BUILDING_H_ */
