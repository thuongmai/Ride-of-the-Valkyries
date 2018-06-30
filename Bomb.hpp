#ifndef BOMB_H
#define BOMB_H

#include "Armament.hpp"

class Bomb : public Armament {
public:

	Bomb(Ogre::SceneManager * _scene_manager, Ogre::String _object_name, float x, float y, float z);
	~Bomb();
	void setOrientation(Ogre::Quaternion target);
	void update(Ogre::Real);
private:
	double time;
};

#endif