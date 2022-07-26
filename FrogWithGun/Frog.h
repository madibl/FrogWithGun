#pragma once
#include <SFML/Graphics.hpp>
#include "Platform.h"
using namespace sf;

class Frog {
private:
	Vector2f position;
	Sprite* sprite;
	//make pass by reference to frog
	float frogSpeed = 6.0f;



public:
	Frog(float startX, float startY, Sprite* associated);
	Vector2f getPosition();
	FloatRect getGlobalBounds();

	void moveLeft();
	void moveRight();
	void fall(float y);
	void jump(float &yVelocity, bool onPlatform);
	void update();
	
	//void gravity(Platform* platform);
	//float calcJump();



};

