#include "EnemyHeli.hpp"

#include <stdio.h>
#include <math.h>

EnemyHeli::EnemyHeli(Ogre::SceneManager* _scene_manager, std::vector<Actor*>* a, std::vector<Ogre::Vector3> patrolRoute, PlayerHeli* _player, Ogre::String _object_name, float x, float y, float z)
	 : Heli(_scene_manager, a, x, y, z, 1.0),  Enemy(_scene_manager, a, _player, _object_name, x, y, z) {
	pos = Ogre::Vector3(x, y, z);
	this->scene_manager = _scene_manager;
	this->arms = a;
	this->name = _object_name;
	this->patrolRoute = patrolRoute;
	Ogre::SceneNode *root = _scene_manager->getRootSceneNode();
	scene_node = createHeli(_scene_manager, name, "EnemyHeliMaterial");
	scene_node->setPosition(pos);

	state = patrol;
	patrolIndex = pDirection = direction = 0;
	timer = 0;
	//scene_node->showBoundingBox(true);
	std::cout << std::endl;
}

EnemyHeli::~EnemyHeli() {

}

void EnemyHeli::update(Ogre::Real inFe) {
	fe = inFe;
	pos = scene_node->getPosition();
	dir = scene_node->_getDerivedOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
	timer += fe;

	//St8 Upd8
	Ogre::Real pSquaredDist = pos.squaredDistance(player->getPosition());
	//finite state machine
	switch (state) {
	case patrol:
		if (pSquaredDist < 200*200) {
			//std::cout << "Switched to attack" << std::endl;	//leash
			state = attack;
			timer = 0;
		}
		goTo(patrolRoute[patrolIndex]);
		if (scene_node->getPosition().squaredDistance(patrolRoute[patrolIndex]) < 100) {
			//std::cout << "Switched to idle" << std::endl;	//pause for a second and go to next patrol route destination
			state = idle;
			direction = loiter;
			timer = 0;
			patrolIndex = (patrolIndex + 1) % patrolRoute.size();
		}
		break;
	case idle:
		if (pSquaredDist < 200*200) {						//leash
			state = attack;
			timer = 0;
			break;
		}
		if (timer > 1) {
			state = patrol;
			timer = 0;
		}
		break;
	case follow:
		break;
	case attack:
		if(pSquaredDist > 400*400) {						//Deleash range
			state = idle;
			timer = 0;
			break;
		}
		if (timer > 1 && pSquaredDist < 300*300) {			//Circling behavior
			pointTo(player->getPosition());
			if (fmod(timer, 5) < 0.02) {					//80% chance to circle the player, 20% to idle, switch every 5 seconds
				int a = rand() % 10;
				if (a <= 3) {
					pDirection = left;
				}
				else if (a <= 7) {
					pDirection = right;
				}
				else {
					pDirection = idle;
				}
			}
			if (wepCD <= 0) {
				if ((rand() % 10) < 5) {						//50% chance to fire at player while circling and iff wepCD is < 0
					fire();
					wepCD = 4;
				}
			}
			direction = pDirection;
		}
		else {												//following behavior
			goTo(player->getPosition());
		}
		break;
	}
	//DO MOVEMENT WORK HERE AND THEN UPDATE VEL
	switch (direction) {
	case Direction::idle:
		break;
	case forward:
		moveForward();
		break;
	case fRight:
		moveForward();
		strafeRight();
		break;
	case right:
		strafeRight();
		break;
	case bRight:
		moveBackward();
		strafeRight();
		break;
	case backward:
		moveBackward();
		break;
	case bLeft:
		moveBackward();
		strafeLeft();
		break;
	case left:
		strafeLeft();
		break;
	case fLeft:
		moveForward();
		strafeLeft();
		break;
	case up:
		moveUp();
		break;
	case down:
		moveDown();
		break;
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

	//SOME SIMPLE "ANIMATING"
	scene_node->getChild(name + "Rotor")->getChild(name + "rBlade")->yaw(Ogre::Radian(Ogre::Degree(15)));
	scene_node->getChild(name + "Tail")->getChild(name + "tBlade")->pitch(Ogre::Radian(Ogre::Degree(-15)));


	if (wepCD > 0) wepCD -= fe;
	pos += vel;
	scene_node->setPosition(pos);
	acc = Ogre::Vector3(0, 0, 0);
	direction = 0;
}

void EnemyHeli::pointTo(Ogre::Vector3 v) {
	Ogre::Vector2 tmp = Ogre::Vector2(v.x - pos.x, v.z - pos.z), tmp2 = Ogre::Vector2(dir.x, dir.z);
	Ogre::Real dif = tmp.x*tmp2.y - tmp.y*tmp2.x;
	if (dif >= 2) {
		scene_node->yaw(Ogre::Degree(turnRate*fe));
	}
	else if (dif <= -2) {
		scene_node->yaw(Ogre::Degree(-turnRate*fe));
	}
}

void EnemyHeli::fireMissile() {
	Ogre::Vector3 tmpd = Ogre::Vector3(pos + dir * 15);
	Missile *msl = new Missile(scene_manager, name + "MSL" + Ogre::StringConverter::toString(mslCount++), tmpd.x, tmpd.y, tmpd.z);
	Ogre::Quaternion tmp = this->scene_node->getOrientation()*Ogre::Vector3::UNIT_Z.getRotationTo(dir);
	msl->setEnemy(true);
	arms->push_back(msl);
	msl->setPHDirection(scene_node->_getDerivedOrientation());
}

void EnemyHeli::goTo(Ogre::Vector3 v) {
	Ogre::Vector2 tmp = Ogre::Vector2(v.x - pos.x, v.z - pos.z), tmp2 = Ogre::Vector2(dir.x, dir.z);
	Ogre::Real dif = tmp.x*tmp2.y - tmp.y*tmp2.x;
	if (dif < 2 && dif > -2) {
		if ((pos.y - v.y) < 2 && (pos.y - v.y) > -2) {
			direction = forward;
		}
		else if (v.y > pos.y) {
			direction = up;
		}
		else if (v.y < pos.y) {
			direction = down;
		}
		return;
	}
	else if (dif >= 2) {
		scene_node->yaw(Ogre::Degree(turnRate*fe));
		return;
	}
	else if (dif <= -2) {
		scene_node->yaw(Ogre::Degree(-turnRate*fe));
		return;
	}
}