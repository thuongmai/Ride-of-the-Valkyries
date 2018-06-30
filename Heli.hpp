#ifndef HELI_H
#define HELI_H

#include "Actor.hpp"
#include "Missile.hpp"
#include "Shell.hpp"
#include "Bomb.hpp"
#include "Mine.hpp"

enum Direction { idle, forward, fRight, right, bRight, backward, bLeft, left, fLeft, up, down }; //enumerator to denote directions. May be useful for heli "AI" or whatnot
enum Behaviour { loiter, patrol, follow, attack };

class Heli : public virtual Actor {
	public:
		Heli(Ogre::SceneManager*, std::vector<Actor*>*, float, float, float, float);
		//????????
	protected:
		const float maxSpeed, acceleration, turnRate;
		Ogre::Vector3 dir, acc;
		int mslCount;
		Ogre::Real wepCD;

		void strafeLeft();
		void strafeRight();
		void moveForward();
		void moveBackward();
		void moveUp();
		void moveDown();
		virtual void fire();
		Ogre::SceneNode* createHeli(Ogre::SceneManager*, Ogre::String, Ogre::String = "def");
};

#endif