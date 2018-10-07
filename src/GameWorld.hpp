#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include "../GameEngine/graphics/includes/Functions.hpp"
#include "../GameEngine/utils/includes/timer.hpp"
#include "../GameEngine/graphics/includes/Camera.hpp"
#include "../GameEngine/utils/includes/Skybox.hpp"
#include "ResourceManager.hpp"
#include "Builder.hpp"
#include "SaveGame.hpp"
#include "StateManager.hpp"
#include "Sound.hpp"

class GameEntity;

const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;

class GameWorld
{
protected:
	std::vector<GameEntity*> mEntityList;
	std::vector< std::vector<int>> m_map;
	GameEntity *m_Player;
    Camera * m_camera;

	SoundManager *mSoundManager;

    GameBoi::graphics::Window *m_window;
    ResourceManager *mResourceManager;
	Builder *mEntityBuilder;
	GameBoi::graphics::Shader *mSkyBoxShader;
	GameBoi::graphics::Skybox *mSkybox;
	

	//game loop vars
	GAME_STATE nextState;
	int mLevel;
	float mMapSize;
	GameBoi::Timer *m_Timer;
	//setting an initial target framerate to 100
	float mFrameRate = 100;
	int mEntityCount = 0;
	GameEntity *m_StartingBlock;
	float mScaling;
	int playerScore = 0;
	unsigned int mCurrentEnemies = 0;
	int mCurrentBombs = 0;
	

public:
	GameWorld(void);
	GameWorld(GameBoi::graphics::Window *window, ResourceManager *resources);
	virtual ~GameWorld(void);
	GameWorld & operator=(const GameWorld & _rhs);
	GameWorld(const GameWorld & _gameworld);

	virtual GAME_STATE runLoop();
	void draw();

	virtual void init(const std::string level);
	void loadGame();
	void saveGame();
	void loadMap(const std::string level);
	void spawnStartEntities();
	bool stepGame(void);
	void renderGame(void);
	void cleanUp(void);
	void playSound(E_SOUNDS sound, bool loop);
	void stopAllSound(void);
	void nextLevel(void);
	void startMusic(void);
	void changeState(GAME_STATE state);
	void quitGame(void);
	int getLevel(void);
	int getScore(void);
	void setScore(int score);
	unsigned int getEnemiesLeft(void);
	void setEnemiesLeft(unsigned int enemies);


	Camera *getCamera();
	GameBoi::graphics::Window *getWindow();


	std::vector<std::vector<int> > getMap(void) const;

	//entities
	std::vector<GameEntity*> *getEntityList();
	void addEntity(GameEntity*);
	Builder* getBuilder(void);
	GameEntity *getPlayer(void);
	GAME_STATE getCurrentState(void);

	//utility
	float getDeltaTime(void);
	float getScaling(void);
	float getMapSize(void);
	void  setMapSize(float mapSize);
	//gameplay functions
	void respawnPlayer();
};