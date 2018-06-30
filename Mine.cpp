#include "Mine.hpp"

Mine::Mine(Ogre::SceneManager* _scene_manager, Ogre::String _object_name, float x, float y, float z) : Armament(scene_manager, x, y, z) {
	this->scene_manager = _scene_manager;
	name = _object_name + Ogre::StringConverter::toString(++count);
	Ogre::Entity* entity = scene_manager->createEntity(name, "SphereMesh");
	entity->setMaterialName("Fuchsia");
	scene_node = scene_manager->getRootSceneNode()->createChildSceneNode(name);
	scene_node->attachObject(entity);
	scene_node->setScale(1, 1, 1);
	scene_node->setPosition(pos);

}
void Mine::setOrientation(Ogre::Quaternion target) {
	scene_node->setOrientation(target);
	scene_node->yaw(Ogre::Degree(90));
}

void Mine::move(Ogre::Vector3 newLocation, Ogre::Real deltaTime) {
	Ogre::Vector3 directionToLocation = newLocation - scene_node->getPosition();

	Ogre::Real speed = 20.0;

	Ogre::Real move = speed*deltaTime;
	Ogre::Vector3 src = scene_node->getOrientation() * Ogre::Vector3::UNIT_Z;
	Ogre::Quaternion quat = src.getRotationTo(directionToLocation);
	/*if ((1.0 + src.dotProduct(directionToLocation)) < 0.0001) {
	scene_node->yaw(Ogre::Degree(180));
	}
	else {
	Ogre::Quaternion quat = src.getRotationTo(directionToLocation);
	scene_node->rotate(quat);
	}*/

	pos = scene_node->getPosition();
	pos -= (scene_node->getOrientation()* Ogre::Vector3::UNIT_Y) *speed*deltaTime;

	scene_node->setPosition(pos);

}


Mine::~Mine() {

}

void Mine::update(Ogre::Real fe) {

}

