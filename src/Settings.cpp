#include "Settings.hpp"
#include "Config.hpp"

Settings::Settings(GameBoi::graphics::Window *window, ResourceManager *resources)
:Menu(window,resources)
{

}


Settings::~Settings(void){
	return;
}

Settings::Settings(const Settings &src):Menu(){
	static_cast<void>(src);
	return;
}

Settings &Settings::operator=(const Settings &rhs){
	static_cast<void>(rhs);
	return *this;
}

 void Settings::init(std::string config)
 {
	config = "";
    // Create and load all the move_left
    m_font = new GameBoi::graphics::Fonts("../Resources/Fonts/press.ttf");
	int xOffset = 200;

	Config *keys = Config::getInstance();


	// ============================== CREATE THE SETTINGS =========================================== //
    Setting *move_up = new Setting(SCREEN_WIDTH/2 - xOffset, SCREEN_HEIGHT/2+300, true, "MOVE UP:       "+config);
	move_up->setValue(std::to_string(char(keys->KEY_UP)));
    mSettings.push_back(move_up);

    Setting *move_down = new Setting(SCREEN_WIDTH/2 - xOffset, SCREEN_HEIGHT/2 + 250, false, "MOVE DOWN:     ");
	move_down->setValue(std::to_string(char(keys->KEY_DOWN)));
    mSettings.push_back(move_down);

	Setting *move_left = new Setting(SCREEN_WIDTH/2 - xOffset, SCREEN_HEIGHT/2 + 200, false, "MOVE LEFT:     ");
	move_left->setValue(std::to_string(char(keys->KEY_LEFT)));
	mSettings.push_back(move_left);

	Setting *move_right = new Setting(SCREEN_WIDTH/2 - xOffset, SCREEN_HEIGHT/2 + 150, false, "MOVE RIGHT:    ");
	move_right->setValue(std::to_string(char(keys->KEY_RIGHT)));
	mSettings.push_back(move_right);

    Setting *drop_bomb = new Setting(SCREEN_WIDTH/2 - xOffset, SCREEN_HEIGHT/2 + 100, false, "DROP BOMB:     ");
	drop_bomb->setValue(std::to_string(char(keys->KEY_DROPBOMB)));
	mSettings.push_back(drop_bomb);

    Setting *fullscreen = new Setting(SCREEN_WIDTH/2 - xOffset, SCREEN_HEIGHT/2, false, "FULLSCREEN:     ");
	fullscreen->setValue(std::to_string(keys->fullscreen));
	mSettings.push_back(fullscreen);

    ResolutionSetting *resolution = new ResolutionSetting(SCREEN_WIDTH/2 - xOffset, SCREEN_HEIGHT/2 - 50, false, "RESOLUTION:     ");
	resolution->changeResolution(keys->res);
	mSettings.push_back(resolution);

    Setting *sound = new Setting(SCREEN_WIDTH/2 - xOffset, SCREEN_HEIGHT/2 - 100, false,     "SOUND:          ");
	sound->setValue(std::to_string(keys->sound));
	mSettings.push_back(sound);

    VolumeSetting *volume = new VolumeSetting(SCREEN_WIDTH/2 - xOffset, SCREEN_HEIGHT/2 - 150, false,     "VOLUME:          ");
	volume->setValue(std::to_string(keys->volume));
	mSettings.push_back(volume);

	Setting *save = new Setting(SCREEN_WIDTH/2 - xOffset - 200, SCREEN_HEIGHT/2 - 300, false,     "SAVE AND EXIT");
	mSettings.push_back(save);

	Setting *exit = new Setting(SCREEN_WIDTH/2 - xOffset + 500, SCREEN_HEIGHT/2 - 300, false,     "BACK");
	mSettings.push_back(exit);


	//=============================== LINK THE SETTINGS ============================================== //
	mSelectedSetting = move_up;

	move_up->nextSetting = move_down;
	move_up->prevSetting = exit;
	move_down->nextSetting = move_left;
	move_down->prevSetting = move_up;
	move_left->prevSetting = move_down;
	move_left->nextSetting = move_right;
	move_right->nextSetting = drop_bomb;
	move_right->prevSetting = move_left;
    drop_bomb->prevSetting = move_right;
    drop_bomb->nextSetting = fullscreen;
    fullscreen->prevSetting = drop_bomb;
    fullscreen->nextSetting = resolution;
    resolution->prevSetting = fullscreen;
    resolution->nextSetting = sound;
	sound->prevSetting = resolution;
	sound->nextSetting = volume;
	volume->prevSetting = sound;
	volume->nextSetting = save;
	save->prevSetting = sound;
	save->nextSetting = exit;
	exit->prevSetting = save;
	exit->nextSetting = move_up;

	std::cout << "SETTINGS INIT END" << std::endl;    
 }

 GAME_STATE Settings::handleInput()
 {
	if (m_Timer->getElapsed() >= 0.25f)
	{
		if (m_window->isKeyPressed(GLFW_KEY_S)) 
		{
			mSelectedSetting = mSelectedSetting->nextSetting;
			m_Timer->resetElapsed();
		}
		else if (m_window->isKeyPressed(GLFW_KEY_W)) {
			mSelectedSetting = mSelectedSetting->prevSetting;
			m_Timer->resetElapsed();		
		}
		else if (m_window->isKeyPressed(GLFW_KEY_ENTER)){
			if (mSelectedSetting->getText() == "BACK"){
				return GAME_STATE::STATE_MENU;
		 	}
			else if (mSelectedSetting->getText().find("FULLSCREEN") != std::string::npos){

				if (mSelectedSetting->getValue() == "0"){
					mSelectedSetting->setValue("1");
				}
				else {
					mSelectedSetting->setValue("0");	
				}

				m_Timer->resetElapsed();
			}
			else if (mSelectedSetting->getText().find("RESOLUTION") != std::string::npos){
				auto *setting = dynamic_cast<ResolutionSetting*>(mSelectedSetting);
				setting->nextResolution();
			}
			else if (mSelectedSetting->getText().find("SOUND") != std::string::npos){
				if (mSelectedSetting->getValue() == "0")
					mSelectedSetting->setValue("1");
				else 
					mSelectedSetting->setValue("0");
			}
			else if (mSelectedSetting->getText().find("VOLUME") != std::string::npos){
				auto *setting = dynamic_cast<VolumeSetting*>(mSelectedSetting);
				setting->nextVolume();
			}
			else if (mSelectedSetting->getText() == "SAVE AND EXIT")
			{
				bool hasNA = false;
				auto config = Config::getInstance();
				for (auto i: mSettings){
					if (i->getValue() == "NA"){
						hasNA = true;
						break;
					}

					if (i->getText().find("MOVE UP") != std::string::npos) 
						config->KEY_UP = std::stoi(i->getValue());
					else if (i->getText().find("MOVE DOWN" )!= std::string::npos)
						config->KEY_DOWN = std::stoi(i->getValue());
					else if (i->getText().find("MOVE RIGHT")!= std::string::npos)
						config->KEY_RIGHT = std::stoi(i->getValue());
					else if (i->getText().find("MOVE LEFT")!= std::string::npos)
						config->KEY_LEFT = std::stoi(i->getValue());
					else if (i->getText().find("DROP BOMB")!= std::string::npos)
						config->KEY_DROPBOMB = std::stoi(i->getValue());
					else if (i->getText().find("SOUND")!= std::string::npos)
						config->sound = std::stoi(i->getValue());
					else if (i->getText().find("VOLUME")!= std::string::npos)
						config->volume = std::stoi(i->getValue());
					else if (i->getText().find("RESOLUTION")!= std::string::npos)
					{
						auto *setting = dynamic_cast<ResolutionSetting*>(i);
						config->res = setting->getCurrentResolution();
						m_window->resize(setting->getResX(), setting->getResY());
					}
					else if (i->getText().find("FULLSCREEN") != std::string::npos)
					{
						config->fullscreen = std::stoi(i->getValue());
						m_window->resize(config->res_x,config->res_y);
					}
				}

				if (!hasNA) {
					Config::getInstance()->saveConfig();
					return GAME_STATE::STATE_MENU;
				}
			}
			else if (mSelectedSetting->getText().find("MOVE UP") != std::string::npos || mSelectedSetting->getText().find("MOVE DOWN") != std::string::npos
			 || mSelectedSetting->getText().find("MOVE LEFT") != std::string::npos || mSelectedSetting->getText().find("MOVE RIGHT") != std::string::npos
			 || mSelectedSetting->getText().find("DROP BOMB") != std::string::npos)
			{
				bool keyPressed = false;
				mSelectedSetting->setValue("PRESS KEY");
				m_Timer->resetElapsed();

				int otherKeys[] = {32, 262, 263, 264, 265};

				while (!keyPressed)
				{
					for (int i = 0; i < 5; i++){
						if (m_window->isKeyPressed(otherKeys[i]) && m_Timer->getElapsed() >= 0.2){

								
							mSelectedSetting->setValue(std::to_string(otherKeys[i]));
							// REMOVE DUPLICATES
							for (auto i : mSettings){
								if (i!= mSelectedSetting && i->getValue() == mSelectedSetting->getValue()){
									i->setValue("NA");
									break;
								}
							}

							keyPressed = true;
							break;
						}						
					}

					for (int i = 64; i < 122; i++)
					{
						if (m_window->isKeyPressed(i) && m_Timer->getElapsed() >= 0.2){
							mSelectedSetting->setValue(std::to_string(i));
							// REMOVE DUPLICATES
							for (auto i : mSettings){
								if (i!= mSelectedSetting && i->getValue() == mSelectedSetting->getValue()){
									i->setValue("NA");
									break;
								}
							}

							keyPressed = true;
							break;
						}
					}
					m_Timer->update();
					m_window->update();
					drawMenu();
				} 
			}
		// 	else if (mSelectedSetting->getText() == "START NEW GAME"){
		// 		return GAME_STATE::STATE_START;
		// 	}
		// 	else if (mSelectedSetting->getText() == "SETTINGS"){
		// 		return GAME_STATE::STATE_SETTINGS;
		// 	}
			m_Timer->resetElapsed();

		}
	}
	return GAME_STATE::STATE_NONE;
     

 }