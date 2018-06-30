#ifndef MISSILE_H
#define MISSILE_H

#include "Armament.hpp"

class Missile : public Armament {
	public:
		Missile(Ogre::SceneManager * _scene_manager, Ogre::String _object_name, float x, float y, float z);
		~Missile();
		void setDirection(Ogre::Quaternion target);
		void setPHDirection(Ogre::Quaternion);
		void update(Ogre::Real);

	
};

#endif