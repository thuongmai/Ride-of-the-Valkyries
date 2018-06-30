#include "Game.hpp"
#include "path_config.h"

namespace Game {
	Game::Game() :
		config_filename_g(""), plugins_filename_g(""), log_filename_g("Ogre.log"),
		window_title("The Game"), custom_window_capacities(""), window_width(1280), window_height(800), window_fullscreen(false),
		viewport_background_color(Ogre::ColourValue(0.0, 0.0, 0.2)), material_directory(MATERIAL_DIRECTORY)
	{
		viewport_width = 0.95f, viewport_height = 0.95f, viewport_left = (1.0f - viewport_width) * 0.5f, viewport_top = (1.0f - viewport_height)*0.5f;
		viewport_z_order = 100;
		camera_near_clip_distance = 0.01, camera_far_clip_distance = 1000.0;
		camera_position = Ogre::Vector3(50.0f, 50.0f, 50.0f), camera_looking_at = Ogre::Vector3(0.0, 0.0, 0.0), camera_up = Ogre::Vector3(0.0, 1.0, 0.0);
		this->lvlSwitch = true;
		timer = 0;
	}
	Game::~Game() {

	}

	void Game::Init() {
		InitRoot();
		InitPlugins();
		InitRenderSystem();
		InitWindow();
		InitEvents();
		InitOIS();
		LoadMaterials();

		world = new World(this->scene_manager);
		world->levelName = &(this->levelName);
		world->timer = &(this->timer);
		InitViewport();
		//scene_manager->setSkyDome(true, "Sky", 1, 1, 600, true, Ogre::Quaternion::IDENTITY, 16, 16, -1, "TheGame"); //broken skydome because I'm not sure what Ogre's SkyDome needs in terms of texture /K
		scene_manager->setSkyBox(true, "Sky", 500);

		InitHUD();
	}

	void Game::InitRoot(void) {
		try {
			ogre_root = std::auto_ptr<Ogre::Root>(new Ogre::Root(config_filename_g, plugins_filename_g, log_filename_g));
			scene_manager = ogre_root->createSceneManager(Ogre::ST_GENERIC, "TheSceneManager");
		}
		catch (Ogre::Exception &e) {
			throw(GameException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch (std::exception &e) {
			throw(GameException(std::string("std::exception: ") + std::string(e.what())));
		}
	}
	void Game::InitPlugins(void) {
		try {
			Strings plugin_names;
			plugin_names.push_back("RenderSystem_GL");
			plugin_names.push_back("Plugin_ParticleFX");

			Strings::iterator iter = plugin_names.begin();
			Strings::iterator iter_end = plugin_names.end();
			for (; iter != iter_end; iter++) {
				Ogre::String& plugin_name = (*iter);
				if (OGRE_DEBUG_MODE) {
					plugin_name.append("_d");
					ogre_root->loadPlugin(plugin_name);
				}
			}
		}
		catch (Ogre::Exception &e) {
			throw(GameException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch (std::exception &e) {
			throw(GameException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}
	void Game::InitRenderSystem(void) {
		try {
			const Ogre::RenderSystemList& render_system_list = ogre_root->getAvailableRenderers();
			if (render_system_list.size() == 0) throw(GameException(std::string("GameException: No rendersystem found")));

			Ogre::RenderSystem *render_system = render_system_list.at(0);
			ogre_root->setRenderSystem(render_system);
		}
		catch (Ogre::Exception &e) {
			throw(GameException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch (std::exception &e) {
			throw(GameException(std::string("std::Exception: ") + std::string(e.what())));
		}

	}
	void Game::InitWindow(void) {
		try {
			ogre_root->initialise(false, window_title, custom_window_capacities);
			Ogre::NameValuePairList params;
			params["FSAA"] = "0";
			params["vsync"] = "true";
			render_window = ogre_root->createRenderWindow(window_title, window_width, window_height, window_fullscreen, &params);
			render_window->setActive(true);
			render_window->setAutoUpdated(false);
		}
		catch (Ogre::Exception &e) {
			throw(GameException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch (std::exception &e) {
			throw(GameException(std::string("std::Exception: ") + std::string(e.what())));
		}

	}
	void Game::InitViewport(void) {				//WILL HAVE TO MOVE CAMERA CREATION TO PLAYER HELI CLASS LATER
		try {
			/* VIEWPORT CREATION */
			PlayerHeli *player = world->getPlayer();
			Ogre::Viewport *viewport = render_window->addViewport(player->getThirdCamera(), viewport_z_order, viewport_left, viewport_top, viewport_width, viewport_height);
			viewport->setAutoUpdated(true);
			viewport->setBackgroundColour(viewport_background_color);
			cThird = true, cSwitch = false;
		}
		catch (Ogre::Exception &e) {
			throw(GameException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch (std::exception &e) {
			throw(GameException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}
	void Game::InitEvents(void) {
		try {
			ogre_root->addFrameListener(this);
			Ogre::WindowEventUtilities::addWindowEventListener(render_window, this);
		}
		catch (Ogre::Exception &e) {
			throw(GameException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch (std::exception &e) {
			throw(GameException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}
	void Game::InitOIS(void) {
		try {
			/* INIT INPUT MANAGER */
			OIS::ParamList pl;
			size_t windowHnd = 0;
			std::ostringstream windowHndstr;
			render_window->getCustomAttribute("WINDOW", &windowHnd);
			windowHndstr << windowHnd;
			pl.insert(std::make_pair(std::string("WINDOW"), windowHndstr.str()));
			pl.insert(std::make_pair(std::string("w64_mouse"), std::string("DISCL_FOREGROUND")));
			pl.insert(std::make_pair(std::string("w64_mouse"), std::string("DISC_NONEXCLUSIVE")));
			input_manager = OIS::InputManager::createInputSystem(pl);

			/* INIT KEYBOARD & MOUSE */
			keyboard = static_cast<OIS::Keyboard*>(input_manager->createInputObject(OIS::OISKeyboard, false));
			mouse = static_cast<OIS::Mouse*>(input_manager->createInputObject(OIS::OISMouse, false));
			unsigned int width, height, depth;
			int top, left;
			render_window->getMetrics(width, height, depth, left, top);
			const OIS::MouseState &ms = mouse->getMouseState();
			ms.width = width;
			ms.height = height;
		}
		catch (std::exception &e) {
			throw(GameException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}
	void Game::LoadMaterials(void) {
		try {
			Ogre::String resource_group_name = "TheGame";
			Ogre::ResourceGroupManager& resource_group_manager = Ogre::ResourceGroupManager::getSingleton();
			resource_group_manager.createResourceGroup(resource_group_name);
			resource_group_manager.addResourceLocation(material_directory, "FileSystem", resource_group_name, false);
			resource_group_manager.initialiseResourceGroup(resource_group_name);
			resource_group_manager.loadResourceGroup(resource_group_name);
		}
		catch (Ogre::Exception &e) {
			throw(GameException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch (std::exception &e) {
			throw(GameException(std::string("std::Exception: ") + std::string(e.what())));
		}
	}
	void Game::InitHUD() {
		Ogre::OverlaySystem* pOverlaySystem = OGRE_NEW Ogre::OverlaySystem();
		this->scene_manager->addRenderQueueListener(pOverlaySystem);
		Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();
		Ogre::FontPtr fnt = Ogre::FontManager::getSingleton().create("chineserocks", "General");
		fnt->setType(Ogre::FontType::FT_TRUETYPE);
		fnt->setSource("chineserocks.ttf");
		fnt->setTrueTypeSize(80);
		fnt->setTrueTypeResolution(96);
		hud = overlayManager.create("Overlay");

		Ogre::OverlayContainer *panel = static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "Wep"));
		panel->setMetricsMode(Ogre::GMM_PIXELS);
		panel->setPosition(0, 600);
		panel->setDimensions(128, 80);
		panel->setMaterialName("TGreen");
		Ogre::TextAreaOverlayElement *wText = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "WepText"));
		wText->setMetricsMode(Ogre::GMM_PIXELS);
		wText->setPosition(64, 0);
		wText->setDimensions(128, 80);
		wText->setCharHeight(80);
		wText->setFontName("chineserocks");
		wText->setColour(Ogre::ColourValue(1, 0, 0.5));
		wText->setCaption(world->getPlayer()->getWep());

		Ogre::OverlayContainer *panel2 = static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "Health"));
		panel2->setMetricsMode(Ogre::GMM_PIXELS);
		panel2->setPosition(1088, 600);
		panel2->setDimensions(128, 80);
		panel2->setMaterialName("TGold");
		Ogre::TextAreaOverlayElement *wText2 = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "HealthText"));
		wText2->setMetricsMode(Ogre::GMM_PIXELS);
		wText2->setPosition(0, 0);
		wText2->setDimensions(128, 80);
		wText2->setCharHeight(80);
		wText2->setFontName("chineserocks");
		wText2->setColour(Ogre::ColourValue(0, 1, 0));
		wText2->setCaption(Ogre::StringConverter::toString(100));

		Ogre::OverlayContainer *panel3 = static_cast<Ogre::OverlayContainer*>(overlayManager.createOverlayElement("Panel", "Title"));
		panel3->setMetricsMode(Ogre::GMM_PIXELS);
		panel3->setHorizontalAlignment(Ogre::GuiHorizontalAlignment::GHA_CENTER);
		panel3->setPosition(-480, 170);
		panel3->setDimensions(960, 80);
		panel3->setMaterialName("TBlue");
		Ogre::TextAreaOverlayElement *tText = static_cast<Ogre::TextAreaOverlayElement*>(overlayManager.createOverlayElement("TextArea", "TitleText"));
		tText->setAlignment(Ogre::TextAreaOverlayElement::Alignment::Center);
		tText->setMetricsMode(Ogre::GMM_PIXELS);
		tText->setPosition(480, 0);
		tText->setDimensions(128, 80);
		tText->setCharHeight(80);
		tText->setFontName("chineserocks");
		tText->setColour(Ogre::ColourValue(1, 1, 0.9));
		tText->setCaption("Ride of the Valkyries");

		hud->add2D(panel);
		hud->add2D(panel2);
		hud->add2D(panel3);
		panel2->addChild(wText2);
		panel->addChild(wText);
		panel3->addChild(tText);
		hud->show();
	}

	void Game::loop() {
		try {
			ogre_root->clearEventTimes();

			while (!render_window->isClosed()) {
				render_window->update(false);
				render_window->swapBuffers();
				ogre_root->renderOneFrame();
			}
		}
		catch (Ogre::Exception &e) {
			throw (GameException(std::string("Ogre::Exception: ") + std::string(e.what())));
		}
		catch (std::exception &e) {
			throw (GameException(std::string("std::exception: ") + std::string(e.what())));
		}
	}

	bool Game::frameRenderingQueued(const Ogre::FrameEvent &fe) {
		keyboard->capture();
		mouse->capture();
		PlayerHeli *player = world->getPlayer();

		//LOGIC
		if (keyboard->isKeyDown(OIS::KC_ESCAPE)) {
			ogre_root->shutdown();
			render_window->destroy();
			return false;
		}

		if (player->getHealth() != 0) {
			player->playerMove(keyboard);
			player->cameraManip(mouse->getMouseState());
		}

		//Camera switching
		if (keyboard->isKeyDown(OIS::KC_0) && !cSwitch) {
			if (cThird) {
				render_window->getViewport(0)->setCamera(world->getPlayer()->getFirstCamera());
				cThird = !cThird;
			}
			else {
				render_window->getViewport(0)->setCamera(world->getPlayer()->getThirdCamera());
				cThird = !cThird;
			}
			cSwitch = true;
		}
		if (!(keyboard->isKeyDown(OIS::KC_0)) && cSwitch)
			cSwitch = !cSwitch;

		timer += fe.timeSinceLastFrame;
		world->update(fe.timeSinceLastFrame);
		if (player->getHealth() != 0) {
			if (timer == 0 && !lvlSwitch) {
				hud->getChild("Title")->getChild("TitleText")->setCaption((levelName == "end") ? "AGNUS DEI QUI TOLLIS PECCATA MUNDI" : levelName);
				hud->getChild("Title")->show();
				lvlSwitch = true;
			}
			else if (timer > 2 && lvlSwitch) {
				if (!(levelName == "end"))
					hud->getChild("Title")->hide();
				lvlSwitch = false;
			}
		}
		else {
			hud->getChild("Title")->getChild("TitleText")->setCaption("REX MORTVVS EST VIVAT REX");
			hud->getChild("Title")->setMaterialName("TRed");
			hud->getChild("Title")->show();
		}

		Ogre::String str = player->getCD() <= 0 ? player->getWep() : Ogre::StringConverter::toString(Ogre::Math::Ceil(player->getCD()));
		int heath = player->getHealth();
		hud->getChild("Wep")->getChild("WepText")->setCaption(str);
		hud->getChild("Health")->getChild("HealthText")->setCaption(Ogre::StringConverter::toString(heath));
		hud->getChild("Health")->getChild("HealthText")->setColour(Ogre::ColourValue(((float)(100 - heath) / (float)100), (float)heath / (float)100, 0));
		return true;
	}

	void Game::windowResized(Ogre::RenderWindow *rw) {
		int width = rw->getWidth();
		int height = rw->getHeight();
		Ogre::Camera* camera = scene_manager->getCamera("TheCamera");

		if (camera != NULL)
			camera->setAspectRatio((double)width / height);
		const OIS::MouseState &ms = mouse->getMouseState();
		ms.width = width;
		ms.height = height;
		render_window->resize(width, height);
		render_window->windowMovedOrResized();
		render_window->update();
	}
}