#include "Tank.hpp"
#include "OGRE/OgreManualObject.h"
#include <OGRE/OgreRoot.h>
#include "OGRE/OgreEntity.h"
#include <exception>

Tank::Tank(Ogre::SceneManager*_scene_manager, std::vector<Actor*>*a, PlayerHeli* _player,Ogre::String _object_name, float x, float y, float z)
	: Enemy(_scene_manager, a, _player, _object_name, x, y, z) {

	name = _object_name;
	this->arms = a;
	scene_manager = _scene_manager;
	Ogre::Entity* entity = scene_manager->createEntity(name, "CubeMesh");
	entity->setMaterialName("EnemyTankMaterial");
	Ogre::Entity* entityTop = scene_manager->createEntity(name + "tankRotor", "tank.mesh");
	scene_node = scene_manager->getRootSceneNode()->createChildSceneNode(name);
	
	scene_node->attachObject(entity);
	scene_node->setScale(10, 10, 20);
	this->pos = Ogre::Vector3(x, 10, z);
	scene_node->setPosition(pos);

	topNode = scene_manager->getSceneNode(object_name)->createChildSceneNode(name + "tankRotor");
	topNode->attachObject(entityTop);
	topNode->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(-90));
	topNode->setScale(Ogre::Vector3(0.5, 0.5, 0.25));
	topNode->translate(Ogre::Vector3(0.0, 0.5, 0.0));
	topNode->setInheritOrientation(false);
	//scene_node->showBoundingBox(true);
	inRange = false;
	maxSpeed = 1.0;
	cooldown = 2.5;
	speed = 1;
	acceleration = 0.1;
	turnRate = 1.0;
	state = patrol;
	patrolIndex = pDirection = direction = 0;
}

Tank::~Tank() {

}

Ogre::SceneNode* Tank::getSceneNode() {
	return scene_node;
}

void Tank::ScanForHeli(void) {
	if (player != NULL) {


		Ogre::Vector3 playerPos = player->getPosition();
		Ogre::Real rangeOfSight = 200;


		double distance = playerPos.squaredDistance(scene_node->getPosition());
		if (distance < 0)
			distance = -distance;

		
		if (playerPos.y < 0)
			playerPos.y = 0;

		
		if (distance < rangeOfSight*rangeOfSight) {

			topNode->lookAt(playerPos, Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_Y);
			inRange = true;
		}
		else {
			topNode->setOrientation(scene_node->getOrientation());
			topNode->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(-90));
			inRange = false;
		}

	}
}
void Tank::pointTo(Ogre::Vector3 v) {
	Ogre::Vector2 tmp = Ogre::Vector2(v.x - pos.x, v.z - pos.z), tmp2 = Ogre::Vector2(dir.x, dir.z);
	Ogre::Real dif = tmp.x*tmp2.y - tmp.y*tmp2.x;
	if (dif >= 3) {
		topNode->yaw(Ogre::Degree(turnRate));
	}
	else if (dif <= -3) {
		topNode->yaw(Ogre::Degree(-turnRate));
	}
	//Ogre::Radian angle;
	//Ogre::Vector3 rotAxis = Ogre::Vector3(1, 0, 1);
	//angle = topNode->getOrientation().s

}

void Tank::MoveToLocation(Ogre::Vector3 v, Ogre::Real deltaTime) {
	//Ogre::Vector2 directionToLocation = newLocation - Ogre::Vector2(scene_node->getPosition().x, scene_node->getPosition().z);

	//Ogre::Real speed = 0.001;

	//Ogre::Real move = speed*deltaTime;
	//Ogre::Vector3 src = scene_node->getOrientation() * Ogre::Vector3::UNIT_Z;

	//if (directionToLocation.length() <= deltaTime*speed) {
	//	scene_node->setPosition(Ogre::Vector3(newLocation.x,pos.y, newLocation.y));
	//}
	//else {


	//	//scene_node->setPosition(scene_node->getPosition() + speed * directionToLocation * deltaTime);
	//	pos += speed * Ogre::Vector3(directionToLocation.x,pos.y , directionToLocation.y) * deltaTime;
	//	scene_node->translate(pos);
	//}
	if (!canMove)
		return;
		Ogre::Vector2 tmp = Ogre::Vector2(v.x - pos.x, v.z - pos.z), tmp2 = Ogre::Vector2(dir.x, dir.z);
		Ogre::Real dif = tmp.x*tmp2.y - tmp.y*tmp2.x;
		if (dif < 3 && dif > -3) {

			direction = forward;

		}
		else if (dif >= 3) {
			scene_node->yaw(Ogre::Degree(turnRate));
			return;
		}
		else if (dif <= -3) {
			scene_node->yaw(Ogre::Degree(-turnRate));
			return;
		}
	
}

void Tank::Patrol(std::vector<Ogre::Vector3> locations)
{

	patrolRoute = locations;
	patrolIndex = 0; // Maybe pick the closest point instead?
}

// Call this each time the object should move (once per frame/turn)


void Tank::ShootHeli() {
	if (inRange) {
		Ogre::Vector3 originPos = topNode->_getDerivedPosition() + (topNode->_getDerivedOrientation() * Ogre::Vector3::UNIT_Y *12) ;
		Shell* shell;

		shell = new Shell(scene_manager, (object_name + "Shell" ), originPos.x,
			originPos.y, originPos.z);
		shell->setEnemy(true);
		arms->push_back(shell);
		shell->setDirection(topNode->_getDerivedOrientation());
		shellCount++;
	}

}

void Tank::moveForward() {
	acc += dir;
}

void Tank::update(Ogre::Real deltaTime) {
	pos = scene_node->getPosition();
	dir = scene_node->_getDerivedOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
	timer += deltaTime;

	//St8 Upd8
	Ogre::Real pSquaredDist = pos.squaredDistance(player->getPosition());
	//finite state machine
	ScanForHeli();
	if (cooldown < 0) {
		ShootHeli();
		cooldown = 2.5;
	}
	cooldown -= deltaTime;
	switch (state) {
	case patrol:

		MoveToLocation(patrolRoute[patrolIndex],deltaTime);
		
		if (scene_node->getPosition().squaredDistance(patrolRoute[patrolIndex]) < 100) {
			//std::cout << "Switched to idle" << std::endl;	//pause for a second and go to next patrol route destination
			state = idle;
			direction = 0;
			timer = 0;
			patrolIndex = (patrolIndex + 1) % patrolRoute.size();
		}
		break;
	case idle:

		if (timer > 1) {
			state = patrol;
			timer = 0;
		}
		break;
	
	}
	
	//DO MOVEMENT WORK HERE AND THEN UPDATE VEL
	switch (direction) {
	case Direction::idle:
		break;
	case forward:
		moveForward();
	}
	if (acc.length() != 0) {
		acc.normalise();
		acc *= acceleration;
	}

	vel += acc;
	if (vel.length() > maxSpeed) {								//max speed limiting
		vel.normalise();
		vel *= maxSpeed;
	}
	if (acc.length() == 0 && vel.length() > 0) {
		vel -= vel.normalisedCopy() * 0.1;
		if (vel.length() < 0.1) vel = Ogre::Vector3(0, 0, 0);	//decelerate if not trying to accelerate.
	}

	pos += vel;
	scene_node->setPosition(pos);
	acc = Ogre::Vector3(0, 0, 0);
	direction = 0;
	
} 