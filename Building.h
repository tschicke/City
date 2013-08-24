/*
 * Building.h
 *
 *  Created on: Aug 23, 2013
 *      Author: Tyler
 */

#ifndef BUILDING_H_
#define BUILDING_H_

class Building {
public:
	Building();
	virtual ~Building();

	void draw();

private:
	int lotWidth, lotDepth;//Buildings locked to lots, or more precise sizes
	int height;
};

#endif /* BUILDING_H_ */
