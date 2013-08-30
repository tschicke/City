/*
 * Noise.h
 *
 *  Created on: Mar 9, 2013
 *      Author: Tyler
 */

#ifndef NOISE_H_
#define NOISE_H_

class Noise {
public:
	Noise();
	Noise(int seed);
	virtual ~Noise();
	double smoothNoise2D(double x, double y);
	double smoothNoise3D(double x, double y, double z);

	double noise1D(int x);
	double noise2D(int x, int y);
	double noise3D(int x, int y, int z);

	double nextDouble();
	float nextFloat();
	int nextInt();

	static const int MAX_NOISE_RAND = 65536;
private:
	double interpolate(double a, double b, double x);

	int seed;
	int counter;
};

#endif /* NOISE_H_ */
