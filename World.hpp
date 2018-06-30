#ifndef WORLD_H
#define WORLD_H

#include <OGRE/Ogre.h>
#include <OGRE/OgreManualObject.h>
#include <OGRE/OgreEntity.h>

#include "PlayerHeli.hpp"
#include "Enemy.hpp"
#include "EnemyHeli.hpp"
#include "Tank.hpp"
#include "Turret.hpp"

typedef std::vector<Enemy*> Enemies;
typedef std::vector<Actor*> Armaments;
//typedef std::vector<Wave*> Waves;

class World {
public:
	World(Ogre::SceneManager*);
	~World();

	void heliHitCheck();
	void enemyHitCheck();
	void EnemeyEnemyCheck();

	void PlayerEnemyCheck();

	void DestroyAllAttachedMovableObjects(Ogre::SceneNode * i_pSceneNode);
	void update(Ogre::Real);		//Need to implement delta time sometime <- Implemented /K
	PlayerHeli* getPlayer();
	void OutOfBoundsCheck();

	Ogre::String* levelName;
	Ogre::Real *timer;
private:
	PlayerHeli *player;
	int maxX;
	int minX;
	int maxY;
	int minY;
	int maxZ;
	int minZ;
	/* For wave based gameplay, I'm thinking we use a "Wave" class that puts enemies into the enemies Enemies vector
	* at the start of each wave or something like that. Then we can simply have a vector or array or w/e of "Waves"
	* that we can iterate through.
	*
	* ^ Ignore that, I'm just gonna write a function here that does the thing, i.e. void loadNextWave(string); and keep a int currentWave data member /K
	*/
	int waveNum, hln, tkn, trn;
	Enemies enemies;
	std::vector<Actor*>* armaments;
	Ogre::SceneManager *scene_manager;
	Ogre::ParticleSystem *particle_system;
	Ogre::Real waveTimer;
	bool interim;

	int loadWave();
};

#endif