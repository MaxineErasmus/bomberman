#include "StateManager.hpp"
#include "GameWorld.hpp"
#include "Menu.hpp"
#include "Settings.hpp"
#include "Config.hpp"
// #include "Sound.hpp"

StateManager::StateManager()
{
	Config *config = Config::getInstance();
    m_window = new GameBoi::graphics::Window("GameBoi!", config->res_x, config->res_y);
	if (config->fullscreen){
		m_window->resize(1920, 1080);
	}

    mCurrentState = NULL;

    // Load all the resources needed
    std::cout << "LOADING RESOURCES" << std::endl;
    loadResources();

}

StateManager::~StateManager(){
return;
}

StateManager::StateManager(const StateManager &src){
	static_cast<void>(src);
	return;
}

StateManager &StateManager::operator=(const StateManager &rhs){
	static_cast<void>(rhs);
	return *this;
}


void StateManager::run(GAME_STATE state)
{
	
    if (mCurrentState != NULL)
        delete mCurrentState;

    if (state == GAME_STATE::STATE_EXIT){
        exit(0);
    }
    else if (state == GAME_STATE::STATE_MENU)
    {
        mCurrentState = new Menu(m_window, &mResourceManager);
        mCurrentState->init("settings.config");
        run(mCurrentState->runLoop());
    }
    else if (state == GAME_STATE::STATE_RESUME){
        mCurrentState = new GameWorld(m_window, &mResourceManager);
    	mCurrentState->loadGame();
		run(mCurrentState->runLoop());
    }
    else if (state == GAME_STATE::STATE_START){
        mCurrentState = new GameWorld(m_window, &mResourceManager);
        mCurrentState->init("1");
        run(mCurrentState->runLoop());
    }   
	else if (state == GAME_STATE::STATE_LEVEL_2){
		mScore = mCurrentState->getScore();
		mCurrentState = new GameWorld(m_window, &mResourceManager);
		mCurrentState->init("2");
		mCurrentState->setScore(mScore);

		run(mCurrentState->runLoop());
	}
	else if (state == GAME_STATE::STATE_LEVEL_3){
		mScore = mCurrentState->getScore();
		mCurrentState = new GameWorld(m_window, &mResourceManager);
		mCurrentState->init("3");
		mCurrentState->setScore(mScore);

		run(mCurrentState->runLoop());
	}
	else if ( state == GAME_STATE::STATE_SETTINGS){
		mCurrentState = new Settings(m_window, &mResourceManager);
		mCurrentState->init("settings.config");
		run(mCurrentState->runLoop());
	}
}

void StateManager::loadResources()
{
	// Shaders
	mResourceManager.loadShader("shader_basic", "../GameEngine/graphics/src/shaders/basic.vert", "../GameEngine/graphics/src/shaders/basic.frag");
	mResourceManager.loadShader("shader_text", "../GameEngine/graphics/src/shaders/text.vert", "../GameEngine/graphics/src/shaders/text.frag");
	mResourceManager.loadShader("shader_clear", "../GameEngine/graphics/src/shaders/clear.vert", "../GameEngine/graphics/src/shaders/clear.frag");

	// ======================================= PLAYER MODEL START ======================================= 
	//standing
	mResourceManager.loadModel("model_player_stand1", "../Resources/player/bboi_stand1.obj");
	mResourceManager.loadModel("model_player_stand2", "../Resources/player/bboi_stand2.obj");
	mResourceManager.loadModel("model_player_stand3", "../Resources/player/bboi_stand3.obj");


	//running
	mResourceManager.loadModel("model_player_run1", "../Resources/player/bboi_run1.obj");
	mResourceManager.loadModel("model_player_run2", "../Resources/player/bboi_run2.obj");
	mResourceManager.loadModel("model_player_run3", "../Resources/player/bboi_run3.obj");

	// ability (bomb placement)
	mResourceManager.loadModel("model_player_place1", "../Resources/player/bboi_bomb1.obj");
	mResourceManager.loadModel("model_player_place2", "../Resources/player/bboi_bomb2.obj");
	mResourceManager.loadModel("model_player_place3", "../Resources/player/bboi_bomb3.obj");

	//dying 
	mResourceManager.loadModel("model_player_die1", "../Resources/player/bboi_die1.obj");
	mResourceManager.loadModel("model_player_die2", "../Resources/player/bboi_die2.obj");
	mResourceManager.loadModel("model_player_die3", "../Resources/player/bboi_die3.obj");
	mResourceManager.loadModel("model_player_die4", "../Resources/player/bboi_die4.obj");


	// ======================================= ENEMY MODEL START ======================================= 

	mResourceManager.loadModel("1-model_enemy", "../Resources/enemy/monster1_run1.obj");
	mResourceManager.loadModel("2-model_enemy", "../Resources/enemy/monster2_run1.obj");
	mResourceManager.loadModel("3-model_enemy", "../Resources/enemy/monster3_run1.obj");


	//running
	mResourceManager.loadModel("1-model_monster_run1", "../Resources/enemy/monster1_run1.obj");
	mResourceManager.loadModel("1-model_monster_run2", "../Resources/enemy/monster1_run2.obj");
	mResourceManager.loadModel("1-model_monster_run3", "../Resources/enemy/monster1_run3.obj");

	mResourceManager.loadModel("2-model_monster_run1", "../Resources/enemy/monster2_run1.obj");
	mResourceManager.loadModel("2-model_monster_run2", "../Resources/enemy/monster2_run2.obj");
	mResourceManager.loadModel("2-model_monster_run3", "../Resources/enemy/monster2_run3.obj");

	mResourceManager.loadModel("3-model_monster_run1", "../Resources/enemy/monster3_run1.obj");
	mResourceManager.loadModel("3-model_monster_run2", "../Resources/enemy/monster3_run2.obj");
	mResourceManager.loadModel("3-model_monster_run3", "../Resources/enemy/monster3_run3.obj");


	//dying 
	mResourceManager.loadModel("1-model_monster_die1", "../Resources/enemy/monster1_die1.obj");
	mResourceManager.loadModel("1-model_monster_die2", "../Resources/enemy/monster1_die2.obj");
	mResourceManager.loadModel("1-model_monster_die3", "../Resources/enemy/monster1_die3.obj");
	mResourceManager.loadModel("1-model_monster_die4", "../Resources/enemy/monster1_die4.obj");

	mResourceManager.loadModel("2-model_monster_die1", "../Resources/enemy/monster2_die1.obj");
	mResourceManager.loadModel("2-model_monster_die2", "../Resources/enemy/monster2_die2.obj");
	mResourceManager.loadModel("2-model_monster_die3", "../Resources/enemy/monster2_die3.obj");
	mResourceManager.loadModel("2-model_monster_die4", "../Resources/enemy/monster2_die4.obj");

	mResourceManager.loadModel("3-model_monster_die1", "../Resources/enemy/monster3_die1.obj");
	mResourceManager.loadModel("3-model_monster_die2", "../Resources/enemy/monster3_die2.obj");
	mResourceManager.loadModel("3-model_monster_die3", "../Resources/enemy/monster3_die3.obj");
	mResourceManager.loadModel("3-model_monster_die4", "../Resources/enemy/monster3_die4.obj");

	// ======================================= BOMB MODEL START ======================================= 

	// (bomb & bomb idle)
	mResourceManager.loadModel("model_bomb", "../Resources/bomb/bombLarge.obj");
	// mResourceManager.loadModel("model_bomb2", "../Resources/bomb/bomb2.obj");



	//walls & crates & death 
	


	// ======================================= WALL MODEL START ======================================= 

	mResourceManager.loadModel("1-model_wall", "../Resources/wall/space_wall.obj");
	mResourceManager.loadModel("2-model_wall", "../Resources/wall/desert_wall.obj");
	mResourceManager.loadModel("3-model_wall", "../Resources/wall/forest_wall.obj");

	// ======================================= FLOOR MODEL START ======================================
	mResourceManager.loadModel("1-model_floor", "../Resources/floor/space_floor.obj");
	mResourceManager.loadModel("2-model_floor", "../Resources/floor/desert_floor.obj");
	mResourceManager.loadModel("3-model_floor", "../Resources/floor/forest_floor.obj");

	// ======================================= CRATE MODEL START ======================================= 
	mResourceManager.loadModel("1-model_crate", "../Resources/crate/space_crate.obj");
	mResourceManager.loadModel("2-model_crate", "../Resources/crate/desert_crate.obj");
	mResourceManager.loadModel("3-model_crate", "../Resources/crate/forest_crate.obj");


	mResourceManager.loadModel("1-model_wall_die1", "../Resources/crate/space_crate_die1.obj");
	mResourceManager.loadModel("1-model_wall_die2", "../Resources/crate/space_crate_die2.obj");
	mResourceManager.loadModel("1-model_wall_die3", "../Resources/crate/space_crate_die3.obj");
	mResourceManager.loadModel("1-model_wall_die4", "../Resources/crate/space_crate_die4.obj");

	mResourceManager.loadModel("2-model_wall_die1", "../Resources/crate/desert_crate_die1.obj");
	mResourceManager.loadModel("2-model_wall_die2", "../Resources/crate/desert_crate_die2.obj");
	mResourceManager.loadModel("2-model_wall_die3", "../Resources/crate/desert_crate_die3.obj");
	mResourceManager.loadModel("2-model_wall_die4", "../Resources/crate/desert_crate_die4.obj");

	mResourceManager.loadModel("3-model_wall_die1", "../Resources/crate/forest_crate_die1.obj");
	mResourceManager.loadModel("3-model_wall_die2", "../Resources/crate/forest_crate_die2.obj");
	mResourceManager.loadModel("3-model_wall_die3", "../Resources/crate/forest_crate_die3.obj");
	mResourceManager.loadModel("3-model_wall_die4", "../Resources/crate/forest_crate_die4.obj");

	// ======================================= PORTAL MODEL START ======================================= 
	
	mResourceManager.loadModel("1-model_portal1", "../Resources/portals/space_portal1.obj");
	mResourceManager.loadModel("1-model_portal2", "../Resources/portals/space_portal2.obj");
	mResourceManager.loadModel("1-model_portal3", "../Resources/portals/space_portal2.obj");
	mResourceManager.loadModel("1-model_portal4", "../Resources/portals/space_portal2.obj");

	mResourceManager.loadModel("2-model_portal1", "../Resources/portals/desert_portal1.obj"); 
	mResourceManager.loadModel("2-model_portal2", "../Resources/portals/desert_portal2.obj"); 
	mResourceManager.loadModel("2-model_portal3", "../Resources/portals/desert_portal3.obj"); 
	mResourceManager.loadModel("2-model_portal4", "../Resources/portals/desert_portal4.obj"); 

	mResourceManager.loadModel("3-model_portal1", "../Resources/portals/forest_portal1.obj");
	mResourceManager.loadModel("3-model_portal2", "../Resources/portals/forest_portal2.obj"); 
	mResourceManager.loadModel("3-model_portal3", "../Resources/portals/forest_portal3.obj"); 
	mResourceManager.loadModel("3-model_portal4", "../Resources/portals/forest_portal4.obj"); 






	//explosion 
	mResourceManager.loadModel("model_explosion", "../Resources/wall/explosion2.obj");

	mResourceManager.loadModel("powerup_speed", "../Resources/powerups/powerup_speed.obj");
	mResourceManager.loadModel("powerup_bomb", "../Resources/powerups/powerup_bomb.obj");
	mResourceManager.loadModel("powerup_life", "../Resources/powerups/powerup_fire.obj");

}