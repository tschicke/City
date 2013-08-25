/*
 * MathHelper.cpp
 *
 *  Created on: Jul 29, 2013
 *      Author: Tyler
 */

#include "MathHelper.h"
#include <cmath>

float * MathHelper::SIN_TABLE = 0;

void MathHelper::init() {
	SIN_TABLE = new float[3600];

	for (int i = 0; i < 3600; ++i) {
		SIN_TABLE[i] = sinf(i * 2.f * MATH_PI / 3600.f);
	}
}

void MathHelper::cleanup() {
	delete SIN_TABLE;
}

float MathHelper::sin_float(float radians) {
	radians = fmodf(radians, 2.0 * MATH_PI);
	if (radians < 0) {
		radians += 2.f * MATH_PI;
	}
	return SIN_TABLE[(int) (radians * 572.9578f)];
}

float MathHelper::cos_float(float radians) {
	radians += toRadians(90);
	radians = fmodf(radians, 2.0 * MATH_PI);
	if (radians < 0) {
		radians += 2.f * MATH_PI;
	}
	return SIN_TABLE[(int) (radians * 572.9578f)];
}

float MathHelper::toRadians(float degrees) {
	return degrees * MATH_PI / 180.f;
}

float MathHelper::toDegrees(float radians) {
	return radians * 180.f / MATH_PI;
}

void MathHelper::bubbleSort(int * list, int numElements){
	while(numElements > 1){
		for(int i = 0; i < numElements - 1; ++i){
			int num1 = list[i];
			int num2 = list[i + 1];

			if(num1 > num2){
				list[i] = num2;
				list[i + 1] = num1;
			}
		}

		numElements--;
	}
}

void MathHelper::reverseBubbleSort(int * list, int numElements){
	while(numElements > 1){
		for(int i = 0; i < numElements - 1; ++i){
			int num1 = list[i];
			int num2 = list[i + 1];

			if(num1 < num2){
				list[i] = num2;
				list[i + 1] = num1;
			}
		}

		numElements--;
	}
}
