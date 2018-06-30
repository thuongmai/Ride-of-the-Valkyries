#ifndef MINE_H
#define MINE_H

#include "Armament.hpp"

class Mine : public Armament {
public:
	Mine(Ogre::SceneManager * _scene_manager, Ogre::String _object_name, float x, float y, float z);
	~Mine();

	void setOrientation(Ogre::Quaternion target);
	void move(Ogre::Vector3 newLocation, Ogre::Real deltaTime);
	void update(Ogre::Real);
};

#endif