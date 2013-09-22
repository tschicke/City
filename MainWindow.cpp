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

#include "MathHelper.h"

MainWindow::MainWindow() {
	width = height = 0;
	running = false;
	blocks = NULL;
}

MainWindow::~MainWindow() {
}

void MainWindow::create(int width, int height, const char* title) {
	this->width = width;
	this->height = height;
	create(sf::VideoMode(width, height, 32), title);

	setFramerateLimit(60);

	initGL();
	MathHelper::init();

	camera.init(glm::vec3(0, 6, 0));
	blocks = new Block*[36];
	for(int x = 0; x < 6; ++x){
		for(int z = 0; z < 6; ++z){
			blocks[z + x * 6] = new Block(x - 3, z - 3);
		}
	}

	Renderer::setProjectionMatrix(90, width, height, 1000);
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

	for(int i = 0; i < 36; ++i){
		delete blocks[i];
	}
	delete[] blocks;
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
//	glEnable(GL_CULL_FACE);
}

void MainWindow::handleInput() {
	sf::Event event;

	static sf::Vector2i lastMousePos = sf::Mouse::getPosition();
	sf::Vector2i mousePos = sf::Mouse::getPosition();
	sf::Vector2i mouseMove = lastMousePos - mousePos;
	lastMousePos = mousePos;

	float moveX = 0, moveY = 0, moveZ = 0;
	int dx = 0, dy = 0;
	while(pollEvent(event)){
		if(event.type == sf::Event::Closed){
			stop();
			return;
		}
		if(event.type == sf::Event::KeyPressed){
			if(event.key.code == sf::Keyboard::Escape){
				stop();
				return;
			}
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		moveZ += 0.4f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		moveZ -= 0.4f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		moveX -= 0.4f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		moveX += 0.4f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
		moveY += 0.4f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
		moveY -= 0.4f;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		dy += 3;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		dy -= 3;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		dx -= 3;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		dx += 3;
	}

	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		dx -= mouseMove.x;
		dy += mouseMove.y;
	}

	camera.move(camera.getMoveVector(moveX, moveY, moveZ) * 3.f);
	camera.rotateWithMove(dx, dy);
}

void MainWindow::update(time_t dt) {
}

void MainWindow::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (int x = 0; x < 6; ++x) {
		for (int z = 0; z < 6; ++z) {
			blocks[z + x * 6]->draw(camera.getViewMatrix());
		}
	}

	display();
}

void MainWindow::cleanup() {
}
