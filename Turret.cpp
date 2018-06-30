#include "Turret.hpp"

Turret::Turret(Ogre::SceneManager*_scene_manager, std::vector<Actor*>* a, PlayerHeli* _player, Ogre::String _object_name, float x, float y, float z, int index)
	: Enemy(_scene_manager, a, _player, _object_name, x, y, z) {
	//Initalize
	isInRange = false;
	missileCount = 0;
	arms = a;
	scene_manager = _scene_manager;
	name = _object_name;
	Ogre::Entity* entity = scene_manager->createEntity(name, "CubeMesh");
	entity->setMaterialName("EnemyTurretMaterial");
	scene_node = scene_manager->getRootSceneNode()->createChildSceneNode(name);
	pos = Ogre::Vector3(x, y, z);
	//Create a top gun
	Ogre::Entity* topEntity = scene_manager->createEntity(name + "top", "tank.mesh");
	topNode = scene_node->createChildSceneNode(name + "top");
	topNode->attachObject(topEntity);
	topNode->setScale(Ogre::Vector3(0.5, 0.5, 0.5));
	topNode->translate(Ogre::Vector3(0.0, 0.5, 0.0));
	topNode->setPosition(0, 0.5, 0);
	topNode->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(60));

	scene_node->attachObject(entity);
	scene_node->setScale(15, 15, 15);
	scene_node->translate(Ogre::Vector3(x, 7.5, z)); //Temp position
	pos.y = 15 / 2;
	//scene_node->translate(Ogre::Vector3(30.0 + index * 30, 0, 0.0 + index * 30)); //Temp position
	cooldown = 2.5;
}

Turret::~Turret() {

}

void Turret::update(Ogre::Real deltaTime) {
	ScanForHeli();
	cooldown -= deltaTime;

	if (cooldown < 0) {
		ShootHeli();
		cooldown = 2.5;
	}
	
	
}

void Turret::ScanForHeli(void) {
	if (player != NULL) {
		Ogre::Vector3 playerPos = player->getPosition();
		Ogre::Real rangeOfSight = MAX_TURRET_RANGE;
		double distance = sqrt(playerPos.squaredDistance(scene_node->getPosition()));
		//std::cout << "++Distance: " << distance << std::endl;
		if (distance < rangeOfSight) {
			isInRange = true;
			topNode->lookAt(playerPos, Ogre::Node::TS_WORLD, Ogre::Vector3::UNIT_Y);
		}
		else {
			isInRange = false;
			topNode->resetOrientation();
			topNode->rotate(Ogre::Vector3(1, 0, 0), Ogre::Degree(60));
		}

	}
}


void Turret::ShootHeli() {
	if (isInRange) {
		Ogre::Vector3 originPos = topNode->_getDerivedPosition() + (topNode->_getDerivedOrientation() * Ogre::Vector3::UNIT_Y * 20);
		Missile* missile;

		missile = new Missile(scene_manager, (name + "Missile" + Ogre::StringConverter::toString(missileCount)), originPos.x,
			originPos.y, originPos.z);
		missile->setEnemy(true);
		arms->push_back(missile);
		missile->setDirection(topNode->_getDerivedOrientation());
		missileCount++;
	}
}