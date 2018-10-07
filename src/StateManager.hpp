#pragma once
#include "../GameEngine/graphics/includes/Functions.hpp"
#include "../GameEngine/graphics/includes/Shader.hpp"
#include "../GameEngine/utils/includes/timer.hpp"
#include "../GameEngine/graphics/includes/Camera.hpp"
#include "../GameEngine/graphics/includes/Models.hpp"
#include "ResourceManager.hpp"
#include <irrKlang.h>

#include <vector>

class GameWorld;

enum GAME_STATE {
    STATE_NONE,
    STATE_EXIT,
    STATE_RESUME,
    STATE_START,
    STATE_LEVEL_2,
    STATE_LEVEL_3,
    STATE_MENU,
    STATE_SETTINGS,
	GAME_WON,
	GAME_OVER
};

class StateManager
{
private: 
    GameWorld *mCurrentState;
    GameBoi::graphics::Window *m_window;
    ResourceManager mResourceManager;

    int mScore =0;

public:
    StateManager();
    ~StateManager();
	StateManager(const StateManager &src);
	StateManager &operator=(const StateManager &rhs);

    void loadResources();
    void run(GAME_STATE state);
};