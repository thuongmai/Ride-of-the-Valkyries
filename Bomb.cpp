#include "Bomb.hpp"

Bomb::Bomb(Ogre::SceneManager* _scene_manager, Ogre::String _object_name, float x, float y, float z) : Armament(scene_manager, x, y, z) {
	this->scene_manager = _scene_manager;
	name = _object_name + Ogre::StringConverter::toString(++count);
	Ogre::Entity* entity = scene_manager->createEntity(name, "CylinderMesh");
	entity->setMaterialName("Gold");
	scene_node = scene_manager->getRootSceneNode()->createChildSceneNode(name);
	scene_node->attachObject(entity);
	scene_node->setScale(0.5, 0.5, 3);
	scene_node->translate(pos);
	speed = 20;
	time = 0;
	vel = 0;

}
void Bomb::setOrientation(Ogre::Quaternion target) {
	scene_node->setOrientation(target);
	
}




Bomb::~Bomb() {

}

void Bomb::update(Ogre::Real fe) {
	pos = scene_node->getPosition();


	pos.y += vel.y *time - 9.8*time*time / 2;


	scene_node->setPosition(pos);
	time += fe;
}

