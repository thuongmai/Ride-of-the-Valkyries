#ifndef SHELL_H
#define SHELL_H

#include "Armament.hpp"

class Shell : public Armament {

public:
	Shell(Ogre::SceneManager * _scene_manager, Ogre::String _object_name, float x, float y, float z);
	~Shell();

	void setDirection(Ogre::Quaternion target);
	void setPHDirection(Ogre::Quaternion);

	void update(Ogre::Real);

private:
	Ogre::Radian angle;
	double time;
	Ogre::Vector3 ori;

};

#endif