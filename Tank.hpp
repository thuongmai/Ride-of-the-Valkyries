#ifndef TANK_H
#define TANK_H

#include "Enemy.hpp"
#include "Shell.hpp"
#include "Bomb.hpp"



class Tank : public Enemy {


	public:

		Tank(Ogre::SceneManager * _scene_manager, std::vector<Actor*>* a, PlayerHeli* _player, Ogre::String _object_name, float x, float y, float z);

		void ScanForHeli(void);

		void pointTo(Ogre::Vector3 v);

		void MoveToLocation(Ogre::Vector3 newLocation, Ogre::Real deltaTime);

		void Patrol(std::vector<Ogre::Vector3> locations);

		Ogre::SceneNode* getSceneNode();

		~Tank();
		void UpdatePatrol(float deltaTime);

		void ShootHeli();

		void moveForward();

		void update(Ogre::Real deltaTime);

private:

	std::vector<Ogre::Vector3> patrolRoute;


	

	Ogre::SceneNode* topNode;

	float maxSpeed, acceleration, turnRate;
	Ogre::Vector3 dir, acc;

	int direction, pDirection,shellCount, state, patrolIndex;

	bool inRange;

	Ogre::Real cooldown, timer;
};

#endif