#include "PlayerHeli.hpp"
#include <stdio.h>


const float PlayerHeli::camera_dist = 75.0;

PlayerHeli::PlayerHeli(Ogre::SceneManager *scene_manager, std::vector<Actor*>*a, float x, float y, float z) : Heli(scene_manager, a, x, y, z, 1.2) {
	this->name = "Player";
	this->scene_manager = scene_manager;
	this->arms = a;
	this->lClickDown = this->eDown = this->qDown = false;
	this->wepCD = this->mslCount = this->mncount = this->bmcount = 0;
	this->pos = Ogre::Vector3(x, y, z);
	this->currWep = missile;
	this->health = 100;
	Ogre::SceneNode *root_node = scene_manager->getRootSceneNode();
	scene_node = createHeli(scene_manager, name, "MainHeliMaterial");

	first = scene_manager->createCamera("FirstCamera");	//first person camera init
	fNode = root_node->createChildSceneNode("fCameraNode");
	fNode->attachObject(first);
	fNode->setInheritOrientation(false);
	first->setNearClipDistance(20);
	first->setFarClipDistance(1000);
	fNode->setPosition(0, 0, 0);
	first->lookAt(Ogre::Vector3(-1, -1, -1));
	//first->setFixedYawAxis(true, Ogre::Vector3(0, 1, 0));

	Ogre::Vector3 tempv = first->getDirection();
	tempv.normalise();
	tempv *= -camera_dist;
	third = scene_manager->createCamera("ThirdCamera:");; //third person camera init
	tNode = fNode->createChildSceneNode("tCameraNode");
	tNode->attachObject(third);
	tNode->setInheritOrientation(false);
	third->setNearClipDistance(0.01);
	third->setFarClipDistance(1000 + camera_dist);
	third->setPosition(tempv);
	third->lookAt(first->getPosition());
	//third->setFixedYawAxis(true, Ogre::Vector3(0, 1, 0));

	vel = Ogre::Vector3(0, 0, 0);

	forCanX = true;
	backCanX = true;
	forCanY = true;
	backCanY = true;
	forCanZ = true;
	backCanZ = true;

	//this->scene_node->showBoundingBox(true);
	scene_node->setPosition(pos);
}

PlayerHeli::~PlayerHeli() {

}

Ogre::Camera* PlayerHeli::getFirstCamera() { return first; }
Ogre::Camera* PlayerHeli::getThirdCamera() { return third; }
Ogre::Real PlayerHeli::getCD() { return wepCD; }
Ogre::Real PlayerHeli::getHealth() { return health; }
Ogre::String PlayerHeli::getWep() {
	switch (currWep) {
	case missile:
		return "MS";
	case mine:
		return "MN";
	case bomb:
		return "BM";
	default:
		return "";
	}
}

void PlayerHeli::damage(int i) {
	health -= i;
	if (health < 0) health = 0;
}

void PlayerHeli::playerMove(OIS::Keyboard* keyboard) {
	pos = scene_node->getPosition();
	dir = scene_node->_getDerivedOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;
	acc = Ogre::Vector3(0, 0, 0);

	//simple movement
	if (keyboard->isKeyDown(OIS::KC_W))
		moveForward();
	else if (keyboard->isKeyDown(OIS::KC_S))
		moveBackward();
	if (keyboard->isKeyDown(OIS::KC_A))
		strafeLeft();
	else if (keyboard->isKeyDown(OIS::KC_D))
		strafeRight();
	if (keyboard->isKeyDown(OIS::KC_SPACE))
		moveUp();
	else if (keyboard->isKeyDown(OIS::KC_LSHIFT))
		moveDown();

	if (keyboard->isKeyDown(OIS::KC_E) && !eDown) {
		if (wepCD <= 0) {
			currWep = (currWep + 1) % 3;
			wepCD = 1;
		}
		eDown = true;
	}
	else if (!keyboard->isKeyDown(OIS::KC_E) && eDown)
		eDown = !eDown;
	else if (keyboard->isKeyDown(OIS::KC_Q) && !qDown) {
		if (wepCD < -0) {
			currWep = (currWep + 5) % 3;
			wepCD = 1;
		}
		qDown = true;
	}
	else if (!keyboard->isKeyDown(OIS::KC_Q) && qDown)
		qDown = !qDown;

	if (acc.length() != 0) {
		acc.normalise();
		acc *= acceleration;
	}
}

void PlayerHeli::cameraManip(OIS::MouseState ms) {
	first->yaw(Ogre::Degree(-ms.X.rel));
	first->pitch(Ogre::Degree(-ms.Y.rel));
	Ogre::Radian fPitch = first->getOrientation().getPitch();
	if (fPitch > Ogre::Radian(Ogre::Math::PI / 2)) first->pitch(Ogre::Degree(ms.Y.rel));
	else if (fPitch < -Ogre::Radian(Ogre::Math::PI / 2)) first->pitch(Ogre::Degree(ms.Y.rel));
	rightDown = ms.buttonDown(OIS::MB_Right);

	if (ms.buttonDown(OIS::MB_Left) && !lClickDown) {
		if (wepCD <= 0) {
			fire();
		}
		lClickDown = true;
	}
	if (!ms.buttonDown(OIS::MB_Left) && lClickDown)
		lClickDown = !lClickDown;
}

void PlayerHeli::update(Ogre::Real inFe) {
	this->fe = inFe;
	pos = scene_node->getPosition();
	Ogre::Real prevX, prevY, prevZ;
	prevX = pos.x;
	prevY = pos.y;
	prevZ = pos.z;
	dir = scene_node->_getDerivedOrientation() * Ogre::Vector3::NEGATIVE_UNIT_Z;

	//rotate player helicopter to camera. hella brokenNOT ANYMORE BITCHESFUCK YEEEAAAAAAHHHHHHH
	if (!rightDown) {
		Ogre::Radian tmp = first->getOrientation().getYaw() - scene_node->_getDerivedOrientation().getYaw();
		if (tmp > Ogre::Radian(Ogre::Math::PI)) tmp -= Ogre::Radian(2 * Ogre::Math::PI);
		else if (tmp < -Ogre::Radian(Ogre::Math::PI)) tmp += Ogre::Radian(2 * Ogre::Math::PI);
		if (tmp < Ogre::Radian(Ogre::Degree(1)) && tmp > Ogre::Radian((Ogre::Degree(-1))))
			scene_node->yaw(Ogre::Degree(tmp));
		if (tmp > Ogre::Radian(Ogre::Degree(1))) scene_node->yaw(Ogre::Degree(turnRate*fe));
		else if (tmp < Ogre::Radian(Ogre::Degree(-1))) scene_node->yaw(Ogre::Degree(-turnRate*fe));
	}
	//SOME SIMPLE "ANIMATING"
	scene_node->getChild(name + "Rotor")->getChild(name + "rBlade")->yaw(Ogre::Radian(Ogre::Degree(15)));
	scene_node->getChild(name + "Tail")->getChild(name + "tBlade")->pitch(Ogre::Radian(Ogre::Degree(-15)));

	vel += acc;
	if (vel.length() > maxSpeed) {
		vel.normalise();
		vel *= maxSpeed;
	}
	if (acc.length() == 0 && vel.length() > 0) {
		vel -= vel.normalisedCopy() * 0.1;
		if (vel.length() < 0.1) vel = Ogre::Vector3(0, 0, 0);
	}

	if (wepCD > 0) wepCD -= fe;
	pos += vel;

	if (forCanX && backCanX && forCanY&& backCanY&& forCanZ&& backCanZ)
		scene_node->setPosition(pos);
	else {
		if (!forCanX)
			if (pos.x - prevX > 0)
				pos.x = prevX;
			else
				forCanX = true;

		if (!forCanY)
			if (pos.y - prevY > 0)
				pos.y = prevY;
			else
				forCanY = true;

		if (!forCanZ)
			if (pos.z - prevZ > 0)
				pos.z = prevZ;
			else
				forCanZ = true;

		if (!backCanX)
			if (pos.x - prevX < 0)
				pos.x = prevX;
			else
				backCanX = true;

		if (!backCanY)
			if (pos.y - prevY < 0)
				pos.y = prevY;
			else
				backCanY = true;

		if (!backCanZ)
			if (pos.z - prevZ < 0)
				pos.z = prevZ;
			else
				backCanZ = true;

		scene_node->setPosition(pos);
	}

	updateCameras();
}

void PlayerHeli::fire() {
	Ogre::Vector3 tmp = Ogre::Vector3(pos - dir * 15) - Ogre::Vector3(0, -5, 0);
	Bomb *bm;
	Mine *mn;
	Missile *ms;
	switch (currWep) {
	case missile:
		ms = new Missile(scene_manager, name + "MSL" + Ogre::StringConverter::toString(mslCount++), pos.x, pos.y, pos.z);
		ms->setPHDirection(scene_node->_getDerivedOrientation());
		arms->push_back(ms);
		wepCD = 2;
		break;
	case bomb:
		bm = new Bomb(scene_manager, name + "BOMB" + Ogre::StringConverter::toString(bmcount++), tmp.x, tmp.y, tmp.z);
		bm->setOrientation(scene_node->_getDerivedOrientation());
		arms->push_back(bm);
		wepCD = 3;
		break;
	case mine:
		mn = new Mine(scene_manager, name + "MINE" + Ogre::StringConverter::toString(mncount++), tmp.x, tmp.y, tmp.z);
		mn->setOrientation(scene_node->_getDerivedOrientation());
		arms->push_back(mn);
		wepCD = 4;
		break;
	}
}

void PlayerHeli::reset() {
	scene_node->setPosition(0, 200, 0);
	vel = Ogre::Vector3(0, 0, 0);
}

void PlayerHeli::updateCameras() {
	first->setPosition(pos);

	Ogre::Vector3 tmp = first->getDirection();
	tmp *= -camera_dist;
	third->setPosition(pos + tmp);
	third->lookAt(scene_node->getPosition());
}

void PlayerHeli::maxXCollision() {
	forCanX = false;
}
void PlayerHeli::minXCollision() {
	backCanX = false;
}
void PlayerHeli::maxYCollision() {
	forCanY = false;
}
void PlayerHeli::minYCollision() {
	backCanY = false;
}
void PlayerHeli::maxZCollision() {
	forCanZ = false;
}
void PlayerHeli::minZCollision() {
	backCanZ = false;
}

void PlayerHeli::collidedEnemey(Ogre::SceneNode* e)
{

	if (scene_node->_getWorldAABB().getMaximum().x > e->_getWorldAABB().getMinimum().x && 
		scene_node->_getWorldAABB().getMinimum().x < e->_getWorldAABB().getMinimum().x) {
		forCanX = false;
	}

	if (scene_node->_getWorldAABB().getMinimum().x < e->_getWorldAABB().getMaximum().x &&
		scene_node->_getWorldAABB().getMaximum().x > e->_getWorldAABB().getMaximum().x) {
		backCanX = false;
	}

	if (scene_node->_getWorldAABB().getMaximum().y > e->_getWorldAABB().getMinimum().y &&
		scene_node->_getWorldAABB().getMinimum().y < e->_getWorldAABB().getMinimum().y) {
		forCanY = false;
	}

	if (scene_node->_getWorldAABB().getMinimum().y < e->_getWorldAABB().getMaximum().y &&
		scene_node->_getWorldAABB().getMaximum().y > e->_getWorldAABB().getMaximum().y) {
		backCanY = false;
	}

	if (scene_node->_getWorldAABB().getMaximum().z > e->_getWorldAABB().getMinimum().z &&
		scene_node->_getWorldAABB().getMinimum().z < e->_getWorldAABB().getMinimum().z) {
		forCanZ = false;
	}

	if (scene_node->_getWorldAABB().getMinimum().z < e->_getWorldAABB().getMaximum().z &&
		scene_node->_getWorldAABB().getMaximum().z > e->_getWorldAABB().getMaximum().z) {
		backCanZ = false;
	}
}

void PlayerHeli::noCollisison() {
	forCanX = true;
	forCanY = true;
	forCanZ = true;
	backCanX = true;
	if(scene_node->_getWorldAABB().getMinimum().y>MIN_Y+2)
		backCanY = true;
	backCanZ = true;
}


