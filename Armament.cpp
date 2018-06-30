#include "Armament.hpp"

Armament::Armament(Ogre::SceneManager* sm, float x, float y, float z) : Actor(x, y, z, 0, 0) {
	enemy = false;
}

Armament::~Armament() {

}

bool Armament::isEnemy() {
	return enemy;
}

void Armament::setEnemy(bool e) {
	enemy = e;
}