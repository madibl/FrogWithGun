#pragma once
#include <SFML/Graphics.hpp>
#include "Frog.h"
using namespace sf;

class Platform {
	//variables
	//func for landing on platforms
private:
	Vector2f platPos;
	int xPlat;
	int yPlat;



public:
	Platform(int xPlat, int yPlat, Sprite* associatedP);
	Sprite* spritePlat;
	Vector2f getPlatPos();
	FloatRect getGlobalBounds();
	//bool onPlatform(Frog* frog, Platform* platform);

};