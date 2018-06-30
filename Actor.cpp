#include "Actor.hpp"


Actor::Actor() {
	pos = Ogre::Vector3(0, 0, 0);
}

Actor::Actor(float x, float y, float z, Ogre::SceneManager* sm, std::vector<Actor*> *arms) {
	pos = Ogre::Vector3(x, y, z);
	scene_manager = sm;
	this->arms = arms;
	collide = false;
	this->fe = 0;
	name = "";
}

Ogre::Vector3 Actor::getPosition() { return pos; }
Ogre::String Actor::getName() { return name; }