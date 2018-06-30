#ifndef ARMAMENT_H
#define ARMAMENT_H

#include "Actor.hpp"
static int count = 0;

class Armament : public Actor {
	public:
		Armament(Ogre::SceneManager*, float, float, float); //floats are initial position
		~Armament();
		bool isEnemy();
		void setEnemy(bool e);
		virtual void update(Ogre::Real) = 0;
	protected:
		bool enemy;

};

#endif