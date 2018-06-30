#ifndef ACTOR_H
#define ACTOR_H

#include <OGRE/OgreSceneNode.h>
#include <OGRE/OgreSceneManager.h>
#include <OGRE/OgreEntity.h>
#include "defs.hpp"


class Actor {
	public:
		Actor();
		Actor(float, float, float, Ogre::SceneManager*, std::vector<Actor*>*);
		Ogre::SceneNode* getSceneNode() { return scene_node; }
		Ogre::Vector3 getPosition();
		Ogre::String getName();
		virtual void update(Ogre::Real) = 0;

	protected:
		std::vector<Actor*> *arms;	//pointer to World's Armament vector to add them there when a thing fires
		Ogre::SceneManager *scene_manager;
		Ogre::SceneNode *scene_node;	//Point to equivalent SceneNode in the scene_manager. Violate MVC architecture mwahahaha.
		Ogre::Vector3 pos, vel;
		Ogre::String name;
		bool collide;
		float speed;
		Ogre::Real fe;
};

#endif