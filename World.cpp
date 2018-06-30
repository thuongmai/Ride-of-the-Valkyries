#include "World.hpp"

World::World(Ogre::SceneManager* scene_manager) {
	this->scene_manager = scene_manager;
	this->armaments = new std::vector<Actor*>;
	this->waveTimer = hln = tkn = trn = 0;
	this->interim = false;
	Ogre::SceneNode *root_node = scene_manager->getRootSceneNode();
	Ogre::Light *global_light = scene_manager->createLight("Light");
	global_light->setType(Ogre::Light::LT_POINT);
	global_light->setDiffuseColour(1, 1, 1);
	global_light->setPosition(0, 500, 0);

	Ogre::MeshManager::getSingleton().createPlane(
		"FloorMesh", "TheGame", Ogre::Plane(Ogre::Vector3::UNIT_Y, 0), 3000, 3000, 20, 20, true, 1, 1, 1, Ogre::Vector3::UNIT_Z);
	Ogre::Entity *floorE = scene_manager->createEntity("GameFloor", "FloorMesh");
	floorE->setMaterialName("Floor");
	root_node->createChildSceneNode()->attachObject(floorE);

	waveNum = 0;
	maxX = 1000;
	minX = -maxX;
	maxY = 1000;
	minY = 0;
	maxZ = 1000;
	minZ = -maxZ;


	//temp or not
#pragma region "CREATE A CUBE (CubeMesh)"
	/*
	Ogre::ManualObject *cube = NULL;
	cube = scene_manager->createManualObject("CubeMesh");
	cube->setDynamic(false);
	cube->begin("Gold", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	//CREATE A CUBE
	float i = 0.5;
	cube->position(i, i, i), cube->normal(i, i, i);
	cube->position(i, -i, i), cube->normal(i, -i, i);
	cube->position(i, -i, -i), cube->normal(i, -i, -i);
	cube->position(i, i, -i), cube->normal(i, i, -i);
	cube->position(-i, i, i), cube->normal(-i, i, i);
	cube->position(-i, -i, i), cube->normal(-i, -i, i);
	cube->position(-i, -i, -i), cube->normal(-i, -i, -i);
	cube->position(-i, i, -i), cube->normal(-i, i, -i);

	cube->quad(0, 1, 2, 3), cube->quad(5, 1, 0, 4), cube->quad(1, 5, 6, 2);
	cube->quad(6, 7, 3, 2), cube->quad(7, 6, 5, 4), cube->quad(0, 3, 7, 4);

	cube->end();
	cube->convertToMesh("CubeMesh");*/
#pragma endregion
#pragma region "CREATE A CUBE 2 USING TRIANGLE (CubeMesh)"
	
	Ogre::ManualObject *cube = NULL;
	cube = scene_manager->createManualObject("CubeMesh");

	cube->setDynamic(false);
	cube->begin("Gold", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	//CREATE A CUBE
	float i = 0.5;		

	//Vertices of a cube
	Ogre::Vector3 v0(-0.5, -0.5,  0.5);
	Ogre::Vector3 v1( 0.5, -0.5,  0.5);
	Ogre::Vector3 v2( 0.5,  0.5,  0.5);
	Ogre::Vector3 v3(-0.5,  0.5,  0.5);
	Ogre::Vector3 v4(-0.5, -0.5, -0.5);
	Ogre::Vector3 v5( 0.5, -0.5, -0.5);
	Ogre::Vector3 v6( 0.5,  0.5, -0.5);
	Ogre::Vector3 v7(-0.5,  0.5, -0.5);

	// Normal of each face of the cube
	Ogre::Vector3 n0( 0.0,  0.0,  1.0);
	Ogre::Vector3 n1( 1.0,  0.0,  0.0);
	Ogre::Vector3 n2( 0.0,  0.0, -1.0);
	Ogre::Vector3 n3(-1.0,  0.0,  0.0);
	Ogre::Vector3 n4( 0.0,  1.0,  0.0);
	Ogre::Vector3 n5( 0.0, -1.0,  0.0);

	// Cube's color
	Ogre::ColourValue clr0(0.0, 0.0, 1.0);
	Ogre::ColourValue clr1(1.0, 0.0, 1.0);
	Ogre::ColourValue clr2(1.0, 1.0, 1.0);
	Ogre::ColourValue clr3(0.0, 1.0, 0.0);
	Ogre::ColourValue clr4(0.0, 0.0, 1.0);
	Ogre::ColourValue clr5(1.0, 0.0, 0.0);
	Ogre::ColourValue clr6(1.0, 1.0, 0.0);
	Ogre::ColourValue clr7(0.0, 1.0, 0.0);

	// This construction only partially uses shared vertices, so that we can assign appropriate vertex normals to each face
	// Each face of the cube is defined by four vertices (with the same normal) and two triangles

	cube->position(v0);
	cube->normal(n0);
	cube->textureCoord(0, 0);
	cube->colour(clr0);

	cube->position(v1);
	cube->normal(n0);
	cube->textureCoord(1, 0);
	cube->colour(clr1);

	cube->position(v2);
	cube->normal(n0);
	cube->textureCoord(1, 1);
	cube->colour(clr2);

	cube->position(v3);
	cube->normal(n0);
	cube->textureCoord(0, 1);
	cube->colour(clr3);

	cube->position(v1);
	cube->normal(n1);
	cube->textureCoord(0, 0);
	cube->colour(clr1);

	cube->position(v5);
	cube->normal(n1);
	cube->textureCoord(1, 0);
	cube->colour(clr5);

	cube->position(v6);
	cube->normal(n1);
	cube->textureCoord(1, 1);
	cube->colour(clr6);

	cube->position(v2);
	cube->normal(n1);
	cube->textureCoord(0, 1);
	cube->colour(clr2);

	cube->position(v5);
	cube->normal(n2);
	cube->textureCoord(0, 0);
	cube->colour(clr5);

	cube->position(v4);
	cube->normal(n2);
	cube->textureCoord(1, 0);
	cube->colour(clr4);

	cube->position(v7);
	cube->normal(n2);
	cube->textureCoord(1, 1);
	cube->colour(clr7);

	cube->position(v6);
	cube->normal(n2);
	cube->textureCoord(0, 1);
	cube->colour(clr6);

	cube->position(v4);
	cube->normal(n3);
	cube->textureCoord(0, 0);
	cube->colour(clr4);

	cube->position(v0);
	cube->normal(n3);
	cube->textureCoord(1, 0);
	cube->colour(clr0);

	cube->position(v3);
	cube->normal(n3);
	cube->textureCoord(1, 1);
	cube->colour(clr3);

	cube->position(v7);
	cube->normal(n3);
	cube->textureCoord(0, 1);
	cube->colour(clr7);

	cube->position(v3);
	cube->normal(n4);
	cube->textureCoord(0, 0);
	cube->colour(clr3);

	cube->position(v2);
	cube->normal(n4);
	cube->textureCoord(1, 0);
	cube->colour(clr2);

	cube->position(v6);
	cube->normal(n4);
	cube->textureCoord(1, 1);
	cube->colour(clr6);

	cube->position(v7);
	cube->normal(n4);
	cube->textureCoord(0, 1);
	cube->colour(clr7);

	cube->position(v1);
	cube->normal(n5);
	cube->textureCoord(0, 0);
	cube->colour(clr1);

	cube->position(v0);
	cube->normal(n5);
	cube->textureCoord(1, 0);
	cube->colour(clr0);

	cube->position(v4);
	cube->normal(n5);
	cube->textureCoord(1, 1);
	cube->colour(clr4);

	cube->position(v5);
	cube->normal(n5);
	cube->textureCoord(0, 1);
	cube->colour(clr5);

	// Add triangles
	for (int i = 0; i < 6; i++){
		cube->triangle(i*4 + 0, i*4 + 1, i*4 + 3);
		cube->triangle(i*4 + 1, i*4 + 2, i*4 + 3);
	}

	cube->end();
	cube->convertToMesh("CubeMesh");
#pragma endregion
#pragma region "CREATE A CYLINDER (CylinderMesh)"
	Ogre::ManualObject *cylinder = NULL;
	cylinder = scene_manager->createManualObject("Cylinder");
	cylinder->setDynamic(false);
	cylinder->begin("Gold", Ogre::RenderOperation::OT_TRIANGLE_LIST);
	//CREATE A CYLINDER
	//Position the vectors
	cylinder->position(0, 0, 1), cylinder->normal(0, 0, 1);
	cylinder->position(0, 0, -1), cylinder->normal(0, 0, -1);
	float theta;
	Ogre::Vector3 vNorm;
	for (int i = 0; i < 30; i++) {	//first circle of vertices
		theta = Ogre::Math::TWO_PI * i / 30;
		vNorm = Ogre::Vector3(cos(theta), sin(theta), 0);

		cylinder->position(Ogre::Vector3(0, 0, 1) + vNorm);
		cylinder->normal(vNorm);
	}
	for (int i = 0; i < 30; i++) {	//second circle of vertices
		theta = Ogre::Math::TWO_PI * i / 30;
		vNorm = Ogre::Vector3(cos(theta), sin(theta), 0);

		cylinder->position(Ogre::Vector3(0, 0, -1) + vNorm);
		cylinder->normal(vNorm);
	}
	//BUILD TRIANGLES & QUAAAAAAAAAAAADS
	for (int i = 0; i < 30; i++) {
		cylinder->triangle(0, (2 + i), (2 + ((i + 1) % 30))); //first circle
		cylinder->quad(2 + i, 2 + 30 + i % 30, 2 + 30 + (i + 1) % 30, 2 + (i + 1) % 30);	//tube
	}
	for (int i = 30; i > 0; i--) {
		cylinder->triangle(1, (2 + (i + 1) % 30 + 30), (2 + i % 30 + 30)); //second circle
	}
	cylinder->end();
	cylinder->convertToMesh("CylinderMesh");
#pragma endregion
#pragma region "CREATE A SPHERE (SphereMesh)"
	//CREATE a Sphere http://www.ogre3d.org/tikiwiki/ManualSphereMeshes
	Ogre::ManualObject* manual = scene_manager->createManualObject("SphereMesh");
	manual->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_TRIANGLE_LIST);

	float fDeltaRingAngle = (Ogre::Math::PI / 16);
	float fDeltaSegAngle = (2 * Ogre::Math::PI / 16);
	unsigned short wVerticeIndex = 0;
	const float r = 2;
	// Generate the group of rings for the sphere
	for (int ring = 0; ring <= 16; ring++) {
		float r0 = r * sinf(ring * fDeltaRingAngle);
		float y0 = r * cosf(ring * fDeltaRingAngle);

		// Generate the group of segments for the current ring
		for (int seg = 0; seg <= 16; seg++) {
			float x0 = r0 * sinf(seg * fDeltaSegAngle);
			float z0 = r0 * cosf(seg * fDeltaSegAngle);
			// Add one vertex to the strip which makes up the sphere
			manual->position(x0, y0, z0);
			manual->normal(Ogre::Vector3(x0, y0, z0).normalisedCopy());
			manual->textureCoord((float)seg / (float)16, (float)ring / (float)16);

			if (ring != 16) {
				// each vertex (except the last) has six indicies pointing to it
				manual->index(wVerticeIndex + 16 + 1);
				manual->index(wVerticeIndex);
				manual->index(wVerticeIndex + 16);
				manual->index(wVerticeIndex + 16 + 1);
				manual->index(wVerticeIndex + 1);
				manual->index(wVerticeIndex);
				wVerticeIndex++;
			}
		}; // end for seg
	} // end for ring
	manual->end();
	manual->convertToMesh("SphereMesh");
#pragma endregion



	player = new PlayerHeli(scene_manager, armaments, 0, 80, 80);
	enemies.push_back(new Turret(scene_manager, armaments, player, "firstturret", 0, 0, -50, 0));
}

World::~World() {
	delete armaments;
}

PlayerHeli* World::getPlayer() { return player; }

void World::OutOfBoundsCheck() {
	Ogre::SceneNode* node;
	for (int i = 0; i < armaments->size(); i++) {
		if (armaments->at(i)->getSceneNode()->getPosition().x < minX ||
			armaments->at(i)->getSceneNode()->getPosition().x > maxX ||
			armaments->at(i)->getSceneNode()->getPosition().y < minY ||
			armaments->at(i)->getSceneNode()->getPosition().y > maxY ||
			armaments->at(i)->getSceneNode()->getPosition().z < minZ ||
			armaments->at(i)->getSceneNode()->getPosition().z > maxZ) {
			node = armaments->at(i)->getSceneNode();
			armaments->erase(armaments->begin() + i);
			//std::cout << node->getName() << " Out of Bound" << "\n";
			DestroyAllAttachedMovableObjects(node);
			scene_manager->destroySceneNode(node);

		}
	}
	for (int i = 0; i < enemies.size(); i++) {
		if (enemies.at(i)->getSceneNode()->getPosition().x < minX)

		if (enemies.at(i)->getSceneNode()->getPosition().x > maxX)

		if (enemies.at(i)->getSceneNode()->getPosition().y < minY)

		if (enemies.at(i)->getSceneNode()->getPosition().y > maxY)

		if (enemies.at(i)->getSceneNode()->getPosition().z < minZ)

		if (enemies.at(i)->getSceneNode()->getPosition().z > maxZ)
			{
								//std::cout << enemies.at(i)->getSceneNode()->getName() << " Out of Bound" << "\n";
			}
	}
	if (player->getSceneNode()->getPosition().x <= MIN_X)
		player->minXCollision();
	if (player->getSceneNode()->getPosition().x >= MAX_X)
		player->maxXCollision();

	if (player->getSceneNode()->_getWorldAABB().getMinimum().y <= MIN_Y + 2)
		player->minYCollision();
	
	if (player->getSceneNode()->_getWorldAABB().getMaximum().y >= MAX_Y)
		player->maxYCollision();
	if (player->getSceneNode()->getPosition().z <= MIN_Z)
		player->minZCollision();
	if (player->getSceneNode()->getPosition().z >= MAX_Z)
		player->maxZCollision();
}

void World::heliHitCheck() {
	Ogre::SceneNode* node, *explode;
	Armament *arm = NULL;
	for (int i = 0; i < armaments->size(); i++) {
		node = armaments->at(i)->getSceneNode();
		arm = (Armament*)armaments->at(i);
		if (arm->isEnemy())
			if (player->getSceneNode()->_getWorldAABB().intersects(node->_getWorldAABB())) {
				explode = scene_manager->getRootSceneNode()->createChildSceneNode("xN" + arm->getName());
				explode->setPosition(arm->getPosition());

				armaments->erase(armaments->begin() + i);
				DestroyAllAttachedMovableObjects(node);
				scene_manager->destroySceneNode(node);
				player->damage(5);

				explode->attachObject(scene_manager->createParticleSystem("ex" + arm->getName(), "explosionTemplate"));
				((Ogre::ParticleSystem*)explode->getAttachedObject("ex" + arm->getName()))->fastForward(1);
			}
	}
}
void World::enemyHitCheck() {
	Ogre::SceneNode* armNode, *enmNode, *explode;
	Armament *arm = NULL;
	for (int i = 0; i < enemies.size(); i++) {
		enmNode = enemies.at(i)->getSceneNode();
		for (int j = 0; j < armaments->size(); j++) {
			arm = (Armament*)armaments->at(j);
			armNode = arm->getSceneNode();
			if (!arm->isEnemy())
				if (enmNode->_getWorldAABB().intersects(armNode->_getWorldAABB())) {
					explode = scene_manager->getRootSceneNode()->createChildSceneNode("xN" + arm->getName());
					explode->setPosition(arm->getPosition());

					armaments->erase(armaments->begin() + j);
					DestroyAllAttachedMovableObjects(armNode);
					scene_manager->destroySceneNode(armNode);
					enemies.erase(enemies.begin() + i);
					DestroyAllAttachedMovableObjects(enmNode);
					scene_manager->destroySceneNode(enmNode);
					//printf("enemy Hit\n");
					//std::cout << enemies.size() << std::endl;
					i++;

					explode->attachObject(scene_manager->createParticleSystem("ex" + arm->getName(), "explosionTemplate"));
					((Ogre::ParticleSystem*)explode->getAttachedObject("ex" + arm->getName()))->fastForward(1);
					break;
				}
		}
	}
}

void World::EnemeyEnemyCheck() {
	Enemy *e1;
	Enemy *e2;
	for (int i = 0; i < enemies.size(); i++) {
		e1 = enemies.at(i);
		for (int j = 0; j < enemies.size(); j++) {
			e2 = enemies.at(j);
			if (e1->getSceneNode()->getName() != e2->getSceneNode()->getName())
				if (e1->getSceneNode()->_getWorldAABB().intersects(e2->getSceneNode()->_getWorldAABB())) {
					e1->collidedEnemey();
					e2->collidedEnemey();
					//printf("Enmies collidded \n");
				}
				else {
					e1->noCollisison();
					e2->noCollisison();
				}
		}
	}
}
void World::PlayerEnemyCheck() {
	Enemy *e;
	bool playerColl = false;
	for (int i = 0; i < enemies.size(); i++) {

		e = enemies.at(i);

		if (player->getSceneNode()->_getWorldAABB().intersects(e->getSceneNode()->_getWorldAABB())) {
			e->collidedEnemey();
			player->collidedEnemey(e->getSceneNode());
			playerColl = true;
		}
		else {
			e->noCollisison();
		
		}
	}
	if (!playerColl)
		player->noCollisison();
}

void World::DestroyAllAttachedMovableObjects(Ogre::SceneNode* i_pSceneNode)
{


	// Destroy all the attached objects
	Ogre::SceneNode::ObjectIterator itObject = i_pSceneNode->getAttachedObjectIterator();

	while (itObject.hasMoreElements())
	{
		Ogre::MovableObject* pObject = static_cast<Ogre::MovableObject*>(itObject.getNext());
		i_pSceneNode->getCreator()->destroyMovableObject(pObject);
	}

	// Recurse to child SceneNodes
	Ogre::SceneNode::ChildNodeIterator itChild = i_pSceneNode->getChildIterator();

	while (itChild.hasMoreElements())
	{
		Ogre::SceneNode* pChildNode = static_cast<Ogre::SceneNode*>(itChild.getNext());
		DestroyAllAttachedMovableObjects(pChildNode);
	}
}

void World::update(Ogre::Real fe) {
	if (interim) {
		if (waveTimer > 0)
			waveTimer -= fe;
		else {
			if (loadWave()) {
				*timer = 0;
				player->reset();
				interim = false;
			}
			else {
				*timer = 0;
				*levelName = "end";
				interim = false;
			}
		}
	}
	else {
		if (enemies.size() == 0) {
			*timer = 0;
			*levelName = "Next wave incoming!";
			interim = true;
			waveTimer = 5;
		}
	}

	player->update(fe);
	
	EnemeyEnemyCheck();
	PlayerEnemyCheck();
	OutOfBoundsCheck();
	enemyHitCheck();
	heliHitCheck();
	for (int i = 0; i < enemies.size(); i++)
		enemies.at(i)->update(fe);
	for (int i = 0; i < armaments->size(); i++)
		armaments->at(i)->update(fe);
}

int World::loadWave() {
	/* Format of text file is in "blocks" initiated by "start" and ended by "end"
	 * If a block is meant to create a helicopter or tank, it starts with a list of Vector3s
	 * denoted by "p v1 v2 v3" where v1 v2 and v3 are the respective components of the vector3
	 * These blocks must end in "heli v1 v2 v3" or "tank v1 v2 v3" where the vector components denote the enemy's spawn position
	 *
	 * A single block can create a set of turrets where it has a list of "trrt v1 v2 v3"
	 *
	 * Quite frankly, just see "wave0.txt" for an example
	 *
	 * When initiating the positions of a turret or a tank, it's y component (v2) doesn't matter
	 * because it'll always spawn and be on the ground anyway	/K
	 */

	hln = tkn = trn = 0;
	std::fstream fs;
	fs.open("wave" + Ogre::StringConverter::toString(waveNum++) + ".txt", std::ifstream::in);
	Ogre::Vector3 pos;
	std::vector<Ogre::Vector3> path;
	Enemy *enemy;
	std::string str, v1, v2, v3;
	std::string title = "";
	//parse the text file yo	/K
	if (fs.is_open()) {
		while (!fs.eof()) {
			//DO STUFF HERE		/K
			fs >> str;
			while (str != "start") {
				title += str + " ";
				fs >> str;
			}
			if (str == "start") {
				while (str != "end") {
					//parse each start/end block	/K
					fs >> str;
					if (str == "p") {
						fs >> v1, fs >> v2, fs >> v3;
						path.push_back(Ogre::Vector3(Ogre::StringConverter::parseVector3(v1 + " " + v2 + " " + v3)));
					}
					else if (str == "heli") {
						fs >> v1, fs >> v2, fs >> v3;
						enemies.push_back(new EnemyHeli(scene_manager, armaments, path, player, "helicopter" + Ogre::StringConverter::toString(waveNum) + "-" + Ogre::StringConverter::toString(hln++), Ogre::StringConverter::parseInt(v1), Ogre::StringConverter::parseInt(v2), Ogre::StringConverter::parseInt(v3)));
					}
					else if (str == "tank") {
						fs >> v1, fs >> v2, fs >> v3;
						Tank *tank = new Tank(scene_manager, armaments, player, "tank" + Ogre::StringConverter::toString(waveNum) + "-" + Ogre::StringConverter::toString(tkn++), Ogre::StringConverter::parseInt(v1), Ogre::StringConverter::parseInt(v2), Ogre::StringConverter::parseInt(v3));
						tank->Patrol(path);
						enemies.push_back(tank);
					}
					else if (str == "trrt") {
						fs >> v1, fs >> v2, fs >> v3;
						enemies.push_back(new Turret(scene_manager, armaments, player, "turret" + Ogre::StringConverter::toString(waveNum) + "-" + Ogre::StringConverter::toString(trn++), Ogre::StringConverter::parseInt(v1), Ogre::StringConverter::parseInt(v2), Ogre::StringConverter::parseInt(v3), trn));
					}
				}
				//Done parsing : clear the path vector for the next enemy	/K
				path.clear();
			}
		}
		*(this->levelName) = title;
		fs.close();
		return 1;
	}
	else {
		return 0;
	}
}