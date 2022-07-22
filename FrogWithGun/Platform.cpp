#include "Platform.h"

Platform::Platform(int xPlat, int yPlat, Sprite* associatedP) {
	spritePlat = associatedP;
	
}

Vector2f Platform::getPlatPos() {
	return spritePlat->getPosition();
}

FloatRect Platform::getGlobalBounds() {
	return spritePlat->getGlobalBounds();
}



