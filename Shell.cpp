#include "Shell.hpp"

Shell::Shell(Ogre::SceneManager* _scene_manager,Ogre::String _object_name, float x, float y, float z) : Armament(_scene_manager, x, y, z) {

	name = _object_name+Ogre::StringConverter::toString(++count);
	scene_manager = _scene_manager;
	Ogre::Entity* entity = scene_manager->createEntity(name, "CylinderMesh");
	entity->setMaterialName("Fuchsia");
	scene_node = scene_manager->getRootSceneNode()->createChildSceneNode(name);
	scene_node->attachObject(entity);
	scene_node->setScale(0.5, 0.5, 3);
	scene_node->translate(pos);
	speed = 80.0;
	vel = 30;
	time = 0;
	ori = scene_node->getOrientation() * Ogre::Vector3::UNIT_Z;
	angle = ori.angleBetween(Ogre::Vector3(ori.x, 0, ori.z));


}

Shell::~Shell() {

}
void Shell::setDirection(Ogre::Quaternion target) {

	scene_node->setOrientation(target);
	scene_node->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(-90));

}

void Shell::setPHDirection(Ogre::Quaternion target) {
	scene_node->setOrientation(target);
	scene_node->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(-180));
	

}



void Shell::update(Ogre::Real fe) {



	pos = scene_node->getPosition();
	//pos += (scene_node->getOrientation() * Ogre::Vector3::UNIT_Z)  * fe;


	pos.y += vel.y*Ogre::Math::Sin(angle)*time - 0.05 * time*time / 2;
	pos.x += vel.x*Ogre::Math::Cos(angle)*time*(scene_node->getOrientation() * Ogre::Vector3::UNIT_Z).x;
	pos.z += vel.z*Ogre::Math::Cos(angle)*time*(scene_node->getOrientation() * Ogre::Vector3::UNIT_Z).z;
	scene_node->pitch(angle*fe, Ogre::Node::TS_LOCAL);
	scene_node->setPosition(pos);
	time += fe;
}