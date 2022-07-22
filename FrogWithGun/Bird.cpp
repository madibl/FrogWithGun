#include "Bird.h"
#include "Bullet.h"



Bird::Bird(float startX, float startY, Sprite* associated) {
	sprite = associated;
}


Vector2f Bird::getPosition() {
	return sprite->getPosition();
}

FloatRect Bird::getGlobalBounds() {
	return sprite->getGlobalBounds();
}

bool Bird::isAlive(int enemyLives) {
	if (enemyLives > 0) {
		return true;
	}
	else {
		return false;
	}
}

//void Bird::gotHit(Bird *bird, Bullet *bullets, int enemyLives) {
//	if (bullets->getPosition().x == bird->getPosition().x) {
//		enemyLives--;
//	}
//}

void Bird::update() {
	sprite->setPosition(position);
}