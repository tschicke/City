/*
 * MainWindow.cpp
 *
 *  Created on: Aug 23, 2013
 *      Author: Tyler
 */

#include "MainWindow.h"

#include <iostream>

#include <gl/glew.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

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

	Renderer::setProjectionMatrix(90, width, height, 100);
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
	glEnable(GL_CULL_FACE);

//	float vertices[] = {
//			0, 0, 0,
//			1, 0, 0,
//			1, 1, 0,
//			0, 1, 0
//	};
//
//	float colors[] = {
//			1, 1, 0,
//			0, 1, 1,
//			1, 0, 1,
//			0, 1, 0
//	};
//
//	float normals[] = {
//			0, 0, 1,
//			0, 0, 1,
//			0, 0, 1,
//			0, 0, 1
//	};
//
//	unsigned int indices[] = {
//			0, 1, 2,
//			0, 2, 3
//	};
//
//	renderer.initShaders("shaders/colorShader.vert", "shaders/colorShader.frag");
//
//	renderer.allocBuffers(sizeof(vertices) + sizeof(colors) + sizeof(normals), sizeof(indices));
//
//	renderer.subVertexData(0, sizeof(vertices), vertices);
//	renderer.subVertexData(sizeof(vertices), sizeof(colors), colors);
//	renderer.subVertexData(sizeof(vertices) + sizeof(colors), sizeof(normals), normals);
//	renderer.subIndexData(0, sizeof(indices), indices);
//
//	renderer.setNumVertices(4, 6);
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

	glm::mat4 viewMatrix = glm::lookAt(glm::vec3(1, 1, 1), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	renderer.render(&viewMatrix);
//	block.draw();

	display();
}

void MainWindow::cleanup() {
}
