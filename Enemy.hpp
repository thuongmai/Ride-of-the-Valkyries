#ifndef ENEMY_H
#define ENEMY_H

#include "Actor.hpp"
#include "PlayerHeli.hpp"

class Enemy : public virtual Actor {
public:
	Enemy(Ogre::SceneManager* sm, std::vector<Actor*>* a, PlayerHeli* _player, Ogre::String _object_name, float x, float y, float z) : Actor(x, y, z, sm, a)
	{
		player = _player; object_name = _object_name; canMove = true;
	};
	void collidedEnemey() { canMove = false; };
	void noCollisison() { canMove = true; };
protected:
	//some sort of pure virtual function to react to player like: void AI(void) = 0; or summat
	PlayerHeli *player;
	Ogre::String object_name;
	bool canMove;

};

#endif