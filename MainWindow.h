/*
 * MainWindow.h
 *
 *  Created on: Aug 23, 2013
 *      Author: Tyler
 */

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_

#include <SFML/Window.hpp>

#include "Common.h"

#include "FirstPersonCamera.h"

//Temp
#include "Block.h"
#include "Renderer.h"

class MainWindow : public sf::Window {
public:
	MainWindow();
	virtual ~MainWindow();

	void create(int width, int height, const char * title);

	void start();
	void stop();
private:
	using sf::Window::create;

	int width, height;

	void run();
	bool running;

	void initGL();

	void handleInput();
	void update(time_t dt);
	void render();

	void cleanup();

	//TEMP
	Block ** blocks;
	ts::FirstPersonCamera camera;
	Renderer renderer;
};

#endif /* MAINWINDOW_H_ */
