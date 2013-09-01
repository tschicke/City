/*
 * MainWindow.cpp
 *
 *  Created on: Aug 23, 2013
 *      Author: Tyler
 */

#include "MainWindow.h"

#include <iostream>

#include <gl/glew.h>

#include "Block.h"

MainWindow::MainWindow() {
	width = height = 0;
	running = false;
}

MainWindow::~MainWindow() {
}

void MainWindow::create(int width, int height, const char* title) {
	this->width = width;
	this->height = height;
	create(sf::VideoMode(width, height, 32), title);

	setFramerateLimit(60);

	initGL();
}

void MainWindow::run() {
	int frames = 0;
	long fpsCounter = 0;

	sf::Clock clock;
	while(running){
		float dt = clock.restart().asMicroseconds();
		fpsCounter += dt;
		frames++;

		handleInput();
		update(dt);
		render();

		if(fpsCounter > 1000000){
			std::cout << "FPS: " << frames << '\n';

			fpsCounter = 0;
			frames = 0;
		}
	}
}

void MainWindow::start() {
	if(running){
		return;
	}

	running = true;

	run();
}

void MainWindow::stop() {
	running = false;
}

void MainWindow::initGL() {
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		std::cerr << "Glew did not initialize properly\n";
	}

	glClearColor(0.8f, 0.5f, 0.1f, 1.f);
//	glClearColor(0, 0, 0, 1);

	glEnable(GL_DEPTH_TEST);
}

void MainWindow::handleInput() {
	sf::Event event;
	while(pollEvent(event)){
		if(event.type == sf::Event::Closed){
			stop();
		}
		if(event.type == sf::Event::KeyPressed){
			if(event.key.code == sf::Keyboard::Escape){
				stop();
			}
		}
	}
}

void MainWindow::update(time_t dt) {
}

void MainWindow::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);

	block.draw();

//	glBegin(GL_TRIANGLES);
//	glColor3f(0, 0, 0);
//	glVertex2f(0, 0);
//	glVertex2f(400, 0);
//	glVertex2f(400, 800);
//
//	glVertex2f(0, 0);
//	glVertex2f(400, 800);
//	glVertex2f(0, 800);
//	glEnd();

	display();
}

void MainWindow::cleanup() {
}
