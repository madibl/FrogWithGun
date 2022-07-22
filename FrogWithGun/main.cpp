// TerminalGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "SFML/Graphics/Text.hpp"
#include <time.h>
#include "Frog.h"
#include "Platform.h"
#include "Bullet.h"
#include "Bird.h"
#include <math.h>
#include <vector>
#include <dos.h>
#include <string>


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
int enemyLives = 1000;


float calcJump() {
	dy += 0.3;
	y += dy;
	return y;

}

//platform function
bool onPlatform(Frog* frog, Platform *platform) {
	//std::cout << platform->getPlatPos().x << ", " << platform->getPlatPos().y << " " << frog->getPosition().x << ", " << frog->getPosition().y <<" " <<prevy<< std::endl;
	/*
	if (prevy == -1 || prevy > platform->getPlatPos().y) return false;
	if (frog->getPosition().y >= platform->getPlatPos().y) std::cout<<"true"<<std::endl;
	if ((frog->getPosition().y >= platform->getPlatPos().y)
		&& (frog->getPosition().x < platform->getPlatPos().x && platform->getPlatPos().x < (frog->getPosition().x + 92))) {
		return true;
	}
	*/
	if (yVelocity >=0 && frog->getGlobalBounds().intersects(platform->getGlobalBounds())) {
		return true;
	}
	else {
		return false;
	}

}

//bool hitsBird(Bird* bird, Bullet) {
//	if (Bullet bullet.getPosition().intersects(bird))
//}


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

		else if (yVelocity != 0) { //on floor jumping
			frog->fall(y + yVelocity);
		}
	 
}






int main()
{
	//create window
	RenderWindow window(VideoMode(960, 540), "Frog with Gun");
	window.setFramerateLimit(45);
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
	frogSprite.setOrigin(0, 46);
	Frog frog(5, 5, &frogSprite);
	
	//enemy
	Sprite birdSprite;
	birdSprite.setTexture(t2);
	birdSprite.setTextureRect(birdRect[0]);
	birdSprite.setPosition(600, 200);
	birdSprite.setScale(5, 5);
	Bird bird(100, 100, &birdSprite);

	Sprite mushroom;
	mushroom.setTexture(t3);
	mushroom.setPosition(200, 200);


	Sprite mushroom2;
	mushroom2.setTexture(t3);
	mushroom2.setPosition(400, 200);

	Sprite mushroom3;
	mushroom3.setTexture(t3);
	mushroom3.setPosition(600, 200);



	std::vector<Platform> platforms;

	Platform platform(100, 200, &mushroom);
	platforms.push_back(platform);
	platforms.push_back(Platform(400, 200, &mushroom2));
	platforms.push_back(Platform(600, 200, &mushroom3));
	platforms.push_back(Platform(600, 200, &mushroom3));

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


	Bullet b1;
	std::vector<Bullet> bullets;

	bullets.push_back(Bullet(b1));




	//while loop to run program while window is open
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
		}
		//yVelocity = calcJump();
		for (int i = 0; i < platforms.size(); i++) {
			gravity(&frog, &platforms[i]);
		}

		
		sf::Vector2f viewMove(0, 0);


		//bullets
		//get end of gun where bullets shoot
		//Length of Vector: |V| = sqrt(V.x ^2 + V.y^2)
		//Normalize vector: U = V / |V|
		gunPoint = Vector2f(frog.getPosition().x + 23, frog.getPosition().y - 23);
		mousePosWindow = Vector2f(Mouse::getPosition(window));
		aimDir = mousePosWindow - gunPoint;
		//normalize vector? and get distance of vector
		aimDirNorm.x = aimDir.x / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
		aimDirNorm.y = aimDir.y / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));

		//possibly a camera
		//if (movingLeft)
		//	viewMove.x -= 0.1f;
		//if (movingRight)
		//	viewMove.x += 0.1f;
	

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
		if (Keyboard::isKeyPressed(Keyboard::Space)) {
			//jumping = true;
			for (int i = 0; i < platforms.size(); i++)
			{
				frog.jump(yVelocity, onPlatform(&frog, &platforms[i]));
			}
			/*if (frog.getPosition().y == 300) {
				yVelocity = -10;
				std::cout <<"jumping " << yVelocity << std::endl;
			}*/
		} 

		//shooting
		if (Mouse::isButtonPressed(Mouse::Left)) {
			b1.bullet.setPosition(gunPoint);
			b1.currVelocity = aimDirNorm * b1.maxSpeed;

			bullets.push_back(Bullet(b1));

		}

		for (size_t i = 0; i < bullets.size(); i++) {

			bullets[i].bullet.move(bullets[i].currVelocity);
		}





		//check if bird is alive
		for (size_t i = 0; i < bullets.size(); i++) {
			if (enemyLives > 0) {
				

				if (birdSprite.getGlobalBounds().contains(bullets[i].getPosition())/*bullets[i].bullet.getPosition().x == birdSprite.getPosition().x && bullets[i].bullet.getPosition().y == birdSprite.getPosition().y*/) {
					enemyLives--;
					bullets.erase(bullets.begin() + i);
					std::cout << "-----------HIT!!------------" << std::endl;

				}
				std::cout << "enemy lives: " << enemyLives << " " << bird.isAlive(enemyLives) << std::endl;
			}
		
			else {
				birdSprite.setPosition(1000, 1000);
				window.close();
				std::cout << "CONGRATS YOU KILLED IT." << std:: endl;
			}
		}
		
		

		//text
		Text healthBar;
		Font myFont;
		myFont.loadFromFile("Textures/OpenSans-Bold.ttf");

		std::string bossHealth = std::to_string(enemyLives);
		healthBar.setFont(myFont);
		healthBar.setString("Boss Bird Health: " + bossHealth);
		healthBar.setCharacterSize(24);
		healthBar.setFillColor(Color::White);
		healthBar.setPosition(600, 200);




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
		birdSprite.setTextureRect(birdRect[birdCount]);
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

		//loop to draw bullets in increments
		for (size_t i = 0; i < bullets.size(); i++) {
			window.draw(bullets[i].bullet);

			if (bullets[i].bullet.getPosition().x < 0 || bullets[i].bullet.getPosition().x > window.getSize().x
				|| bullets[i].bullet.getPosition().y < 0 || bullets[i].bullet.getPosition().y > window.getSize().y) {
				bullets.erase(bullets.begin() + i);
			}

		}

		//trying to move bird up and down
		/*int moveY = 200;
		if (birdSprite.getPosition() ) {
			
			moveY += 10;

			birdSprite.setPosition(600, moveY);
			

		}
		else if (birdSprite.getPosition().y > 0) {
			moveY -= 10;
			
		}*/
		window.draw(birdSprite);
		
		/*for (Platform &p : platforms) {
			window.draw(*p.spritePlat);
		}*/
		for (int i = 0; i < platforms.size(); i++) {
			window.draw(*platforms[i].spritePlat);
		}
		//window.draw(mushroom);
		window.draw(healthBar);
		window.display();
	}

}

