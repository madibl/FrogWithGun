// TerminalGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "SFML\Window.hpp"
#include "SFML\System.hpp"
#include <time.h>
#include "Frog.h"
#include "Platform.h"
#include "Bullet.h"
#include <math.h>


using namespace sf;

struct point {
	int x, y;
};

//kinda out of place but here is a thing to deal with jumping mechanics

int x;
int y;
int h;
int prevy=-1;
float dx;
float dy; //change in y
float yVelocity = 0.0f;

float calcJump() {
	dy += 0.3;
	y += dy;
	return y;

}

//platform function
bool onPlatform(Frog* frog, Platform* platform) {
	if (prevy == -1 || prevy > platform->getPlatPos().y) return false;

	std::cout << platform->getPlatPos().x << ", "<<platform->getPlatPos().y<<" "<<frog->getPosition().x<<", "<<frog->getPosition().y<< std::endl;
	if ((frog->getPosition().y >= platform->getPlatPos().y)
		&& (frog->getPosition().x < platform->getPlatPos().x && platform->getPlatPos().x < (frog->getPosition().x - 92))) {
		return true;
	}
	return false;
}


//gravity function
void gravity(Frog* frog, Platform* platform) {
	
	/*if (frog->getPosition().y > 300 || onPlatform(frog, platform)) {
		yVelocity = 0.0f;
		frog->fall(300);
		std:: cout << "yVelocity = " << yVelocity << std::endl;
	}*/

	if (onPlatform(frog, platform)) {
		yVelocity = 0;
		frog->fall(platform->getPlatPos().y);
		std::cout << "on platform " << y << std::endl;
	}

	else if (frog->getPosition().y > 300) { //below floor
		yVelocity = 0.0f;
		frog->fall(300);
	}

	else if (frog->getPosition().y < 300) { //above floor
		yVelocity += 0.3f;
		frog->fall(y += yVelocity);
	}

	else if (yVelocity!=0) { //on floor jumping
		frog->fall(y + yVelocity);
	}
	 
	

}






int main()
{
	//create window
	RenderWindow window(VideoMode(960, 540), "Frog with Gun");
	window.setFramerateLimit(60);
	sf::View camera(sf::Vector2f(480, 270), sf::Vector2f(960, 540));
	camera.setCenter(480,270);

	//get sprites and animations
	Texture t1, t2, t3, t4, t5, t6;
	t1.loadFromFile("Textures/FrogWalkRight.png");
	t2.loadFromFile("Textures/birdfly.png");
	t3.loadFromFile("Textures/mushPlat1.png");
	t4.loadFromFile("Textures/background.png");
	t5.loadFromFile("Textures/FrogWalkLeft.png");


	//arays for frog animation
	IntRect frogRect[3];
	frogRect[0] = IntRect(0, 0, 46, 46);
	frogRect[1] = IntRect(0, 46, 46, 46);
	frogRect[2] = IntRect(0, 94, 46, 46);

	IntRect frogRectL[3];
	frogRectL[0] = IntRect(0, 0, 46, 46);
	frogRectL[1] = IntRect(0, 46, 46, 46);
	frogRectL[2] = IntRect(0, 94, 46, 46);

	//array for bird animation
	IntRect birdRect[2];
	birdRect[0] = IntRect(0, 0, 46, 46);
	birdRect[1] = IntRect(0, 46, 46, 46);



	Sprite frogSprite;
	Frog frog(5, 5, &frogSprite);
	

	Sprite bird;
	bird.setTexture(t2);
	bird.setTextureRect(birdRect[0]);
	bird.setPosition(100, 100);

	Sprite mushroom;
	mushroom.setTexture(t3);
	mushroom.setPosition(200, 200);
	point plat[20];

	Platform platform(100, 200, &mushroom);

	Sprite background;
	background.setTexture(t4);

	Clock frogclock;
	int frogCount = 0;
	float frogTimer = 0;

	Clock birdclock;
	int birdCount = 0;
	float birdTimer = 0;

	//vars for gun
	Vector2f gunPoint;
	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;




	//other variables
	bool movingRight = false;
	bool movingLeft = false;
	bool jumping = false;
	bool idle = false;



	//while loop to run program while window is open
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
		}
		//yVelocity = calcJump();
		gravity(&frog, &platform);

		sf::Vector2f viewMove(0, 0);

		if (movingLeft)
			viewMove.x -= 0.1f;
		if (movingRight)
			viewMove.x += 0.1f;
	

		//std::cout << frog.getPosition().y << std::endl;
		//move right
		if (Keyboard::isKeyPressed(Keyboard::D)) {
			frogSprite.setTexture(t1);
			frogSprite.setTextureRect(frogRect[0]);
			movingRight = true;
			frog.moveRight();
		}

		//moveLeft
		if (Keyboard::isKeyPressed(Keyboard::A)) {
			frogSprite.setTexture(t5);
			frogSprite.setTextureRect(frogRectL[0]);
			movingLeft = true;
			frog.moveLeft();
		}

		//jumping
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			jumping = true;
			if (frog.getPosition().y == 300) {
				yVelocity = -10;
				std::cout <<"jumping " << yVelocity << std::endl;
			}
		}



		//bullets
		//get end of gun where bullets shoot
		gunPoint = Vector2f(frog.getPosition().x + 46, 23);
		mousePosWindow = Vector2f(Mouse::getPosition(window));
		aimDir = mousePosWindow - gunPoint;
		//aimDirNorm = aimDir / (sqrt(pow(aimDir.x, 2) + pow(aimDir, 2)));

		//std::cout << aimDirNorm.x << " " << aimDirNorm.y << "\n";



		Bullet b1;

		

		//initialize clock to cycle through animation
		frogSprite.setTextureRect(frogRect[frogCount]);
		frogTimer = frogclock.getElapsedTime().asSeconds();
		if (frogTimer > 0.1f) {
			if (frogCount < 2 && movingRight) {
				frogCount++;
			}
			else {
				frogCount = 0;
			}
			frogclock.restart();
		}


		//bird animation
		bird.setTextureRect(birdRect[birdCount]);
		birdTimer = birdclock.getElapsedTime().asSeconds();
		if (birdTimer > 0.1f) {
			if (birdCount < 1) {
				birdCount++;
			}
			else {
				birdCount = 0;
			}
			birdclock.restart();
		}



		//draw things
		//camera.move(viewMove);
		//window.setView(camera);
		window.clear();
		window.draw(background);
		window.draw(frogSprite);
		window.draw(bird);
		window.draw(mushroom);
		window.display();
		
		prevy = frog.getPosition().y;
	}

}

