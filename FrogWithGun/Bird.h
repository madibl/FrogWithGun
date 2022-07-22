#pragma once
#include <SFML/Graphics.hpp>
#include "Bullet.h"
using namespace sf;

class Bird {
private:
	Vector2f position;
	Sprite* sprite;

public: 
	Bird(float startX, float startY, Sprite* associated);
	Vector2f getPosition();
	FloatRect getGlobalBounds();
	bool isAlive(int enemyLives);
	//void gotHit(Bird* bird, Bullet* bullets, int enemyLives);
	void update();


};