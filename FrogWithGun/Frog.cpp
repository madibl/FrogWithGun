#pragma once
#include "Frog.h"
//#include "platform.h"

Frog::Frog(float startX, float startY, Sprite* associated) {
	position.x = startX;
	position.y = startY;

	sprite = associated;

	int x = 100, y = 100, h = 200;
	float dx = 0, dy = 0;
}


Vector2f Frog::getPosition() {
	return sprite->getPosition();
}


void Frog::moveLeft() {
	position.x -= frogSpeed;
	sprite->setPosition(position);
}
void Frog::moveRight() {
	position.x += frogSpeed;
	sprite->setPosition(position);
}


void Frog::update() {
	sprite->setPosition(position);
}

void Frog::fall(float y) {
	position.y = y;
	sprite->setPosition(position);
}

void Frog::jump(float &yVelocity) {
	if (sprite->getPosition().y == 300)
		yVelocity = -10;
}