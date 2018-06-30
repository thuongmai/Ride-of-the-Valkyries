#ifndef ENEMYHELI_H
#define ENEMYHELI_H

#include "Heli.hpp"
#include "Enemy.hpp"



class EnemyHeli : public virtual Heli, public virtual Enemy {
	public:
		EnemyHeli(Ogre::SceneManager * _scene_manager, std::vector<Actor*>*, std::vector<Ogre::Vector3>, PlayerHeli * _player, Ogre::String _object_name, float x, float y, float z);
		~EnemyHeli();
		void update(Ogre::Real);
	private:
		//meccha nemui private members
		std::vector<Ogre::Vector3> patrolRoute;
		int patrolIndex, state, direction, pDirection;
		Ogre::Real timer;

		//private functions
		void goTo(Ogre::Vector3);
		void pointTo(Ogre::Vector3);
		void fireMissile();
};

#endif