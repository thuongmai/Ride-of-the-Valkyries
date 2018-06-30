#ifndef GAME_H
#define GAME_H

#include <OGRE/OgreRoot.h>
#include "OGRE/OgreRenderSystem.h"
#include "OGRE/OgreRenderWindow.h"
#include "OGRE/OgreWindowEventUtilities.h"
#include "OGRE/OgreManualObject.h"
#include "OGRE/OgreEntity.h"
#include "OGRE/Overlay/OgreOverlayContainer.h"
#include "OGRE/Overlay/OgreOverlayManager.h"
#include "OGRE/Overlay/OgreOverlaySystem.h"
#include "OGRE/Overlay/OgreTextAreaOverlayElement.h"
#include "OGRE/Overlay/OgreFontManager.h"
#include "OIS/OIS.h"
#include <exception>

#include "World.hpp"

typedef std::vector<Ogre::String> Strings;

namespace Game {

	class GameException : public std::exception {
		public:
			GameException(std::string message) : message_(message) {};
			virtual const char* what() const throw() { return message_.c_str(); }
		private:
			std::string message_;

	};

	class Game :
		public Ogre::FrameListener,
		public Ogre::WindowEventListener
		{
			public:
				Game();
				~Game();
				void Init();
				void loop();

			private:
				//MEMBERS
				//Config stuff
				const Ogre::String config_filename_g, plugins_filename_g, log_filename_g;
				//Window settings
				const Ogre::String window_title, custom_window_capacities;
				const unsigned int window_width, window_height;
				const bool window_fullscreen;
				//Viewport & Camera Init settings
				float viewport_width, viewport_height, viewport_left, viewport_top, camera_near_clip_distance, camera_far_clip_distance;
				unsigned short viewport_z_order;
				const Ogre::ColourValue viewport_background_color;
				Ogre::Vector3 camera_position, camera_looking_at, camera_up;
				//Materials file name thing w/e
				const Ogre::String material_directory;
				bool cThird, cSwitch, lvlSwitch;

				Ogre::String levelName;

				std::auto_ptr<Ogre::Root> ogre_root;
				Ogre::SceneManager *scene_manager;
				Ogre::RenderWindow *render_window;
				OIS::InputManager *input_manager;
				OIS::Keyboard *keyboard;
				OIS::Mouse *mouse;
				World *world;

				Ogre::Overlay *hud;
				Ogre::Real timer;

				//FUNCTIONS
				//Init Functions
				void InitRoot(void);
				void InitPlugins(void);
				void InitRenderSystem(void);
				void InitWindow(void);
				void InitViewport(void);	//
				void InitEvents(void);
				void InitOIS(void);
				void LoadMaterials(void);
				void InitHUD();


				bool frameRenderingQueued(const Ogre::FrameEvent&);
				void windowResized(Ogre::RenderWindow *rw);
	};
}

#endif GAME_H