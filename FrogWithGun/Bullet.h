#pragma once
#include <SFML/Graphics.hpp>
using namespace sf;

class Bullet {
public:
	CircleShape bullet;
	Vector2f currVelocity;
	float maxSpeed;

	Bullet(float radius = 2.0f) : currVelocity(0.0f, 0.0f), maxSpeed(15.0f) {
		this->bullet.setRadius(radius);
		this->bullet.setFillColor(Color::White);
	}
	
	Vector2f getPosition();

};