#ifndef PLAYERHELI_H
#define PLAYERHELI_H

#include "Heli.hpp"

#include <OIS/OIS.h>

enum Weapon { missile, bomb, mine};

class PlayerHeli : public Heli {
	public:
		PlayerHeli(Ogre::SceneManager*, std::vector<Actor*>*, float, float, float);
		~PlayerHeli();
		void update(Ogre::Real);
		void reset();
		void damage(int);
	
		Ogre::Camera* getFirstCamera();
		Ogre::Camera* getThirdCamera();
		Ogre::Real getCD();
		Ogre::Real getHealth();
		Ogre::String getWep();
	
		void playerMove(OIS::Keyboard*);
		void cameraManip(OIS::MouseState);

		
		void maxXCollision();
		void minXCollision();
		void maxYCollision();
		void minYCollision();
		void maxZCollision();
		void minZCollision();
		void noCollisison();
		void collidedEnemey(Ogre::SceneNode* e);
	
	private:
		Ogre::Camera *third, *first;
		Ogre::SceneNode* tNode, *fNode;
		static const float camera_dist;				//dist of third person camera from player model
		bool rightDown, lClickDown, eDown, qDown, forCanX, backCanX, forCanY, backCanY, forCanZ, backCanZ;;
		int currWep, mncount, bmcount, health;

		void fire();
		void updateCameras();


};


#endif