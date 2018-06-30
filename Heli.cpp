#include "Heli.hpp"

Heli::Heli(Ogre::SceneManager* sm, std::vector<Actor*>* a, float x, float y, float z, float ms) : maxSpeed(ms), acceleration(0.1), turnRate(60),  Actor(x, y, z, sm, a) {
	wepCD = 0;
}


void Heli::moveForward() {
	acc += dir;
}
void Heli::moveBackward() {
	acc += -dir;
}
void Heli::strafeLeft() {
	acc += Ogre::Vector3(dir.z, 0, -dir.x);
}
void Heli::strafeRight() {
	acc += Ogre::Vector3(-dir.z, 0, dir.x);
}
void Heli::moveUp() {
	acc += Ogre::Vector3(0, 1, 0);
}
void Heli::moveDown() {
	acc += Ogre::Vector3(0, -1, 0);
}

void Heli::fire() {
	Ogre::Vector3 tmpd = Ogre::Vector3(pos + dir * 15);
	Missile *msl = new Missile(scene_manager, name + "MSL" + Ogre::StringConverter::toString(mslCount++), tmpd.x, tmpd.y, tmpd.z);
	arms->push_back(msl);
	msl->setPHDirection(scene_node->_getDerivedOrientation());
	msl->setEnemy(true);
	wepCD = 2;
}

Ogre::SceneNode* Heli::createHeli(Ogre::SceneManager* scene_manager, Ogre::String string, Ogre::String color) {
	try {
		Ogre::SceneNode *root_node = scene_manager->getRootSceneNode();
		bool cusColor = (color != "def");

		Ogre::Entity *heli = scene_manager->createEntity("CubeMesh");			//main body
		heli->setMaterialName(cusColor ? color : "Fuchsia");
		Ogre::SceneNode *new_node = root_node->createChildSceneNode(string + "Chassis");
		new_node->attachObject(heli);
		new_node->setPosition(0, 0, 0);
		new_node->scale(10, 14, 30);

		Ogre::SceneNode *nose_node = new_node->createChildSceneNode(string + "Nose");
		nose_node->setInheritScale(false);
		Ogre::Entity *nose = scene_manager->createEntity("CubeMesh");
		nose->setMaterialName(cusColor ? color : "Fuchsia");
		nose_node->attachObject(nose);
		nose_node->setPosition(0, -0.25, -0.6167);
		nose_node->scale(10, 7, 7);

		Ogre::SceneNode *rotor_node = new_node->createChildSceneNode(string + "Rotor");	//rotor child of body
		rotor_node->setInheritScale(false);
		Ogre::Entity *rotor = scene_manager->createEntity("CylinderMesh");
		rotor->setMaterialName(cusColor ? color.append("X") : "Blue");
		rotor_node->attachObject(rotor);
		rotor_node->scale(3, 3, 1);
		rotor_node->pitch(Ogre::Radian(Ogre::Math::TWO_PI / 4));
		rotor_node->setPosition(0, 0.538, 0);

		Ogre::SceneNode *rBlade_node = rotor_node->createChildSceneNode(string + "rBlade");//rotor blade child of rotor
		rBlade_node->setInheritScale(false);
		Ogre::Entity *blade = scene_manager->createEntity("CylinderMesh");
		blade->setMaterialName(cusColor ? color : "Green");
		rBlade_node->attachObject(blade);
		rBlade_node->scale(1, 0.5, 35);
		rBlade_node->pitch(Ogre::Radian(Ogre::Math::TWO_PI / 4));
		rBlade_node->yaw(Ogre::Radian(Ogre::Math::TWO_PI / 4));
		rBlade_node->setPosition(0, 0, 0);

		Ogre::SceneNode *tail_node = new_node->createChildSceneNode(string + "Tail");	//tail child of body
		tail_node->setInheritScale(false);
		Ogre::Entity* tail = scene_manager->createEntity("CylinderMesh");
		tail->setMaterialName(cusColor ? color : "Gold");
		tail_node->attachObject(tail);
		tail_node->setPosition(0, 0, 1);
		tail_node->scale(3, 3, 15);

		Ogre::SceneNode *tblade_node = tail_node->createChildSceneNode(string + "tBlade");//tail blade
		tblade_node->setInheritScale(false);
		Ogre::Entity *tblade = scene_manager->createEntity("CylinderMesh");
		tblade->setMaterialName(cusColor ? color : "Red");
		tblade_node->attachObject(tblade);
		tblade_node->scale(0.5, 1, 10);
		tblade_node->pitch(Ogre::Radian(Ogre::Math::TWO_PI / 4));
		tblade_node->setPosition(-1.08, 0, 0.95);

		return new_node;
	}
	catch (Ogre::Exception &e) {
		throw(e);
	}
}