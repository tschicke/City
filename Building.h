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
	int lotWidth, lotDepth;
	int height;
};

#endif /* BUILDING_H_ */
