#ifndef TURRET_H
#define TURRET_H

#include "Enemy.hpp"
#include "Shell.hpp"
#include "defs.hpp"

class Turret : public Enemy {
	public:
		Turret(Ogre::SceneManager * _scene_manager, std::vector<Actor*>*, PlayerHeli * _player, Ogre::String _object_name, float x, float y, float z, int index);
		~Turret();
		void update(Ogre::Real);
		void ScanForHeli(void);
		void ShootHeli();
		//void ShootHeli(Ogre::Real deltaTime);
	private:
		//samidare wo atsumete hayashi mogami gawa
		Ogre::SceneManager* scene_manager;
		Ogre::SceneNode* topNode;
		Ogre::String object_name;
		bool isInRange;
		int missileCount;
		Ogre::Real cooldown;
		std::vector<Missile*> missiles;
};

#endif