#include "../GameEngine/graphics/includes/Functions.hpp"
#include "../GameEngine/graphics/includes/Shader.hpp"
#include "../GameEngine/utils/includes/timer.hpp"
#include "../GameEngine/graphics/src/Textures.cpp"
#include "../GameEngine/graphics/includes/Camera.hpp"
#include "../GameEngine/graphics/includes/Models.hpp"

#include "Config.hpp"
#include "Entity.hpp"
#include "ResourceManager.hpp"
#include "GameWorld.hpp"
#include "Menu.hpp"
#include "StateManager.hpp"

#include <iostream>
	
int main() 
{
	//GameWorld *gameWorld = new GameWorld(m_window);
	//gameWorld->init("dank.map");
	//gameWorld->runLoop();
	Config *config = Config::getInstance();
	config->loadConfig("../Resources/settings.conf");
	StateManager stateManager;
	stateManager.run(GAME_STATE::STATE_MENU);
}