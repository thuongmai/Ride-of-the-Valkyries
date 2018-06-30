#include "Missile.hpp"

Missile::Missile(Ogre::SceneManager* _scene_manager,Ogre::String _object_name,  float x, float y, float z) : Armament(_scene_manager, x,y,z) {
	name = _object_name;
	this->scene_manager = _scene_manager;
	Ogre::Entity* entity = scene_manager->createEntity(name, "CylinderMesh");
	entity->setMaterialName("Gold");
	scene_node = scene_manager->getRootSceneNode()->createChildSceneNode(name);
	scene_node->attachObject(entity);
	scene_node->setScale(0.5, 0.5, 3);
	scene_node->translate(pos);

	speed = 180.0;
	vel = 0;
}

Missile::~Missile() {

}
void Missile::setDirection(Ogre::Quaternion target){

	scene_node->setOrientation(target);
	scene_node->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(-90));
}
void Missile::setPHDirection(Ogre::Quaternion target) {
	scene_node->setOrientation(target);
	scene_node->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(-180));

}

void Missile::update(Ogre::Real fe) {
	pos = scene_node->getPosition();
	pos += (scene_node->getOrientation() * Ogre::Vector3::UNIT_Z) * speed * fe;

	scene_node->setPosition(pos);

}