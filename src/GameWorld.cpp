#include "GameWorld.hpp"
#include "GameEntity.hpp"
#include "Font.hpp"
#include "../GameEngine/utils/includes/Skybox.hpp"
#include <fstream>
#include "Config.hpp"
#include <unistd.h>

GameWorld::GameWorld(void){
	return;
}

GameWorld::GameWorld(GameBoi::graphics::Window *window, ResourceManager *resources)
{
	// setup the builder
	mEntityBuilder = new Builder();
	mEntityBuilder->create(this, resources);
	mResourceManager = resources;
	// setup the camera
	m_camera = new Camera(glm::vec3(4.0f, 6.0f, 0.0f));
	m_camera->LookAt(glm::vec3(4.0f, 1.0f, 4.0f));
	// setup the window
	m_window = window;
	nextState = GAME_STATE::STATE_NONE;


	// SKYBOX
	mSkyBoxShader = new GameBoi::graphics::Shader("../GameEngine/graphics/src/shaders/skybox.vert", "../GameEngine/graphics/src/shaders/skybox.frag");

}

void GameWorld::quitGame(void)
{
	for (auto i : mEntityList)
	{
		std::cout << "killing entity: " << i->getType() << std::endl;
		i->destroy();
	}
	cleanUp();
}

bool GameWorld::stepGame(void)
{

	bool pause = false;
	//exit the game
	if (m_window->isKeyPressed(GLFW_KEY_ESCAPE))
	{
		pause = true;
		saveGame();
	}
	// std::cout << "STEP" << std::endl;
	m_camera->Position = glm::vec3(m_Player->getPosition()->x, m_Player->getPosition()->y + 6.0f * mScaling, m_Player->getPosition()->z - 5.0f * mScaling);
	for (auto i : mEntityList)
	{
		i->update();
	}
	// std::cout << "CLEANUP" << std::endl;
	cleanUp();
	// std::cout << "CLEANUP COMPLETE" << std::endl;

	// std::cout << "STEP COMPLETE" << std::endl;

	return pause;
}

void GameWorld::startMusic(void){
	if (mLevel == 1) {
		playSound(E_SOUNDS::E_SOUNDS_LEVEL_1, true);
	} else if (mLevel == 2) {
		playSound(E_SOUNDS::E_SOUNDS_LEVEL_2, true);
	}
	else if (mLevel == 3) {
		playSound(E_SOUNDS::E_SOUNDS_LEVEL_3, true);
	}
}

void GameWorld::renderGame(void)
{

	for (auto i : mEntityList)
	{
		auto entityGraphics = dynamic_cast<GraphicsComponent *>(i->getComponent(COMPONENT_GRAPHICS));
		if (entityGraphics != NULL)
		{
			entityGraphics->render();
			// m_window->drawHitbox(i->getPosition()->x,i->getPosition()->z,mScaling);
		}
	}
}

GAME_STATE GameWorld::runLoop()
{

	std::cout << "GAMEWORLD: runLoop() start" << std::endl;

	startMusic();
	m_Timer->Reset();

	float timeStep = 0.0f;
	float Delta = 0.0f;
	float renderTime = 0;
	int frameCount = 0;
	float sumTimes = 0.0f;
	int engineTick = 0.0f;
	int dropcounter = 0;
	int last = 0;
	std::string fps = "0";
	std::string engine = "0";

	//skybox setup
	GameBoi::graphics::Fonts font("../Resources/Fonts/Chicago3D.ttf");
	GameBoi::graphics::Fonts font2("../Resources/Fonts/Chicago3D.ttf");
	GameBoi::graphics::Fonts font3("../Resources/Fonts/Chicago3D.ttf");
	// PlayerMovementComponent* playerMovement = dynamic_cast<PlayerMovementComponent*>(m_Player->getComponent(COMPONENT_PLAYERMOVEMENT));

	//GameBoi::graphics::Shader skyboxShader("../GameEngine/graphics/src/shaders/skybox.vert", "../GameEngine/graphics/src/shaders/skybox.frag");
	// std::vector<std::string> faces{
	// 	"../Resources/skybox/space/red/bkg1_right1.png",
	// 	"../Resources/skybox/space/red/bkg1_left2.png",
	// 	"../Resources/skybox/space/red/bkg1_top3.png",
	// 	"../Resources/skybox/space/red/bkg1_bottom4.png",
	// 	"../Resources/skybox/space/red/bkg1_front5.png",
	// 	"../Resources/skybox/space/red/bkg1_back6.png"};

	// GameBoi::graphics::Skybox Box(faces);
	// skyboxShader.setInt("skybox", 0);
	//skybox end
	m_Timer->update();

	while (!m_window->closed())
	{
		m_Timer->update();
		Delta = m_Timer->DeltaTime();
		renderTime = Delta;
		//catch up by dropping the frame rate to compensate for long render times or speeding it up when leftover ticks
		if (renderTime < 1.0f / mFrameRate)
		{
			dropcounter--;
			// std::cout << "\033[0;32m";
			if (dropcounter == -mFrameRate && mFrameRate <= 500)
			{
				last += 2;
				mFrameRate += 2.0f + last;
				dropcounter = 0;
			}
		}
		else
		{
			// std::cout << "\033[0;31m";
			dropcounter++;
			if (dropcounter > mFrameRate)
				dropcounter -= mFrameRate / 2;
			mFrameRate -= 2.0f;
			last = 0;
		}
		while (timeStep < (1.0f / mFrameRate))
		{
			//update timer sets start time to now
			if (stepGame())
				nextState = GAME_STATE::STATE_MENU;
			timeStep += Delta;
			m_Timer->update();
			Delta = m_Timer->DeltaTime();
			engineTick++;
		}
		m_Timer->Reset();
		//clear screen before render
		m_window->clear();
		//reset timer so we can include how long render takes
		//render it all
		renderGame();

		// draw skybox as last
		glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
		
		mSkyBoxShader->use();
		glm::mat4 view = m_camera->GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(m_camera->Zoom), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
		view = glm::mat4(glm::mat3(m_camera->GetViewMatrix())); // remove translation from the view matrix
		
		mSkyBoxShader->setMat4("view", view);
		mSkyBoxShader->setMat4("projection", projection);

		mSkybox->Draw();

		if (nextState != GAME_STATE::STATE_NONE)
		{
			if (nextState == GAME_STATE::GAME_WON)
			{
				// font.RenderText(*mResourceManager->getShader("shader_text"), "YOU WON WELL DONE!", Config::getInstance()->res_x / 2, Config::getInstance()->res_y / 2, 1, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);
				font3.RenderText(*mResourceManager->getShader("shader_text"), "CONGRATULATIONS YOU WON!", Config::getInstance()->res_x / 2 - 100, Config::getInstance()->res_y / 2, 1, glm::vec3(1, 1, 0), Config::getInstance()->res_x, Config::getInstance()->res_y);
				m_window->update();
				sleep(3);
				nextState = GAME_STATE::STATE_MENU;
			}
			else if (nextState == GAME_STATE::GAME_OVER)
			{
				// font.RenderText(*mResourceManager->getShader("shader_text"), "YOU WON WELL DONE!", Config::getInstance()->res_x / 2, Config::getInstance()->res_y / 2, 1, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);
				font3.RenderText(*mResourceManager->getShader("shader_text"), "GAME OVER!!!", Config::getInstance()->res_x / 2 - 100, Config::getInstance()->res_y / 2, 1, glm::vec3(1, 1, 0), Config::getInstance()->res_x, Config::getInstance()->res_y);
				m_window->update();
				sleep(3);
				nextState = GAME_STATE::STATE_MENU;
			}
			stopAllSound();
			return nextState;
		}
		
		font2.RenderText(*mResourceManager->getShader("shader_text"), fps + " FPS", 0, SCREEN_HEIGHT - 50, 1, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);
		font.RenderText(*mResourceManager->getShader("shader_text"), std::to_string(playerScore) + " SCORE", SCREEN_WIDTH - 300, SCREEN_HEIGHT - 50, 1, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);
		font2.RenderText(*mResourceManager->getShader("shader_text"), std::to_string(m_Player->getLives()) + " LIVES", SCREEN_WIDTH - 300, SCREEN_HEIGHT - 100, 1, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);
		font2.RenderText(*mResourceManager->getShader("shader_text"), std::to_string(mCurrentEnemies) + " ENEMIES", SCREEN_WIDTH - 300, SCREEN_HEIGHT - 150, 1, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);
		font2.RenderText(*mResourceManager->getShader("shader_text"), std::to_string( static_cast<int>(m_Player->getBombRange())) + " BOMB POWER", SCREEN_WIDTH - 300, SCREEN_HEIGHT - 200, 1, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);


		m_window->update();
		sumTimes += timeStep;
		if (sumTimes >= 1.0f)
		{
			fps = std::to_string(int(frameCount / sumTimes));
			engine = std::to_string(engineTick / sumTimes);
			//std::cout << "FPS[" << fps << "]/ENGINE[" << engine << "]" << std::endl;
			// std::cout << "PLAYER X:" << m_Player->getPosition()->x << " Z: " << m_Player->getPosition()->z << std::endl;
			// std::cout << "\033[0m";
			sumTimes = 0;
			engineTick = 0;
			frameCount = 0;
		}
		timeStep = 0;
		frameCount++;
	}
	stopAllSound();
	return GAME_STATE::STATE_EXIT;
}

float GameWorld::getMapSize(void)
{
	return static_cast<float>(m_map.size());
}

void GameWorld::setMapSize(float mapSize)
{
	mMapSize = mapSize;
}

GameWorld::~GameWorld(void)
{
	std::cout << "GameWorld decontructed" << std::endl;
	return;
}

GameWorld &GameWorld::operator=(const GameWorld &_rhs)
{
	std::cout << "Assignment constructor\n";
	this->m_map = _rhs.getMap();
	return *this;
}

GameWorld::GameWorld(const GameWorld &_gameworld)
{
	std::cout << "COPY CONSTRUCTOR\n";
	this->m_map = _gameworld.getMap();
	return;
}

GameEntity *GameWorld::getPlayer(void)
{
	return m_Player;
}

void GameWorld::loadMap(const std::string level)
{
	std::cout << "init the game world with map " << level << std::endl;
	std::string myLine;

	std::ifstream myFile("../Resources/maps/" + level);
	if (myFile.is_open())
	{
		while (getline(myFile, myLine))
		{
			std::vector<int> line;
			for (int i = 0; myLine[i]; i++)
			{
				line.push_back(myLine[i]);
			}
			m_map.push_back(line);
			line.clear();
		}
	}
	else
	{
		std::cout << "an error occured opening map file" << std::endl;
		exit(1);
		return;
	}
	return;
}

void GameWorld::spawnStartEntities()
{
	float scaling = mResourceManager->getScaling();
	scaling *= 2.0f;
	std::cout << "Largest x = " << scaling << std::endl;
	mScaling = scaling;
	mEntityBuilder->setGameScale(scaling);
	std::vector<std::vector<int>> map = this->m_map;

	// read map from gameworld into game entities
	for (unsigned int x = 0; x < map.size(); x++)
	{
		for (unsigned int y = 0; y < map[x].size(); y++)
		{
			mEntityBuilder->createFloor(x * scaling, -2.0f, y * scaling);
			//if the number read is a 1 (ascii 49)
			if (map[x][y] == 49)
			{
				mEntityBuilder->createWall(x * scaling, 0, y * scaling, false);
			}
			// //if the number read is 2 destroyable wall
			if (map[x][y] == 50)
			{
				mEntityBuilder->createWall(x * scaling, 0, y * scaling, true);
			}
			//if the number read is 3
			if (map[x][y] == 51)
			{
				mEntityBuilder->createEnemy(x * scaling, 0.1, y * scaling);
			}
			// //if the number read is 4
			if (map[x][y] == 52)
			{
				//create the starting block for the player
				m_StartingBlock = mEntityBuilder->createStartingGate(x * scaling, 0.1, y * scaling);
			}
			// //if the number read is 5 end block
			if (map[x][y] == 53)
			{
				//create the starting block for the player
				mEntityBuilder->createEndWall(x * scaling, 0, y * scaling, true);
			}
		}
	}
	if (!m_StartingBlock)
	{
		std::cout << "PLAYER MISSING FROM MAP" << std::endl;
		exit(1);
	}
	m_Player = mEntityBuilder->createPlayer(m_StartingBlock->getPosition()->x, m_StartingBlock->getPosition()->y, m_StartingBlock->getPosition()->z);
}

int GameWorld::getLevel(void)
{
	return mLevel;
}

void GameWorld::loadGame(void)
{
	SaveGame read;
	std::ifstream ifs("lastSession.data", std::ios::binary);
	std::vector<std::vector<int>> map = this->m_map;

	for (unsigned int x = 0; x < map.size(); x++)
	{
		for (unsigned int y = 0; y < map[x].size(); y++)
		{
			mEntityBuilder->createFloor(x * mScaling, -2.0f, y * mScaling);
		}
	}

	if (ifs)
	{
		float scaling = mResourceManager->getScaling();
		scaling *= 2.0f;
		mScaling = scaling;

		double Xpos;
		double Ypos;
		double Zpos;
		int type;

		while (ifs.read((char *)&read, sizeof(read)))
		{
			type = read.type;
			if (type == 1)
			{
				mLevel = read.level;
				mEntityBuilder->setLevel(std::to_string(mLevel));
				mEntityBuilder->setGameScale(scaling);
			}
		}
		std::ifstream ifs_2("lastSession.data", std::ios::binary);

		while (ifs_2.read((char *)&read, sizeof(read)))
		{
			// read map from gameworld into game entities
			type = read.type;
			Xpos = static_cast<double>(read.Xpos);
			Zpos = static_cast<double>(read.Zpos);
			Ypos = static_cast<double>(read.Ypos);

			std::cout << "TYPE : " << type << std::endl;
			// //if the number read is a 1 (ascii 49)
			if (type == 1)
			{

				m_Player = mEntityBuilder->createPlayer(Xpos, Ypos, Zpos);

				auto moveComponent = dynamic_cast<PlayerMovementComponent *>(m_Player->getComponent(E_COMPONENT_TYPE::COMPONENT_MOVEMENT));

				m_Player->setBombRange(read.range);
				m_Player->setLives(read.lives);
				moveComponent->setSpeed(read.speed);
				setScore(read.score);
				bool alive = read.isAlive;
				if (!alive)
					m_Player->kill();

			}
			if (type == 2)
			{
				GameEntity *enemy = mEntityBuilder->createEnemy(Xpos, Ypos, Zpos);
				auto moveComponent = dynamic_cast<EnemyMovementComponent *>(enemy->getComponent(E_COMPONENT_TYPE::COMPONENT_MOVEMENT));

				moveComponent->setDirection(read.xDir, read.zDir);
				bool alive = read.isAlive;
				if (!alive)
					enemy->kill();

			}
			if (type == 3)
			{
				mEntityBuilder->createWall(Xpos, Ypos, Zpos, false);
			}
			if (type == 5)
			{
				GameEntity *bomb = mEntityBuilder->createBomb(Xpos, Ypos, Zpos);
				auto timer = dynamic_cast<TimerComponent *>(bomb->getComponent(E_COMPONENT_TYPE::COMPONENT_TIMER));
				timer->setElapsed(read.elapsedTime);
			}
			if (type == 4)
			{
				mEntityBuilder->createWall(Xpos, Ypos, Zpos, true);
			}
			if (type == 6)
			{
				m_StartingBlock = mEntityBuilder->createStartingGate(Xpos, Ypos, Zpos);
			}
			if (type == 7)
			{
				mEntityBuilder->createPowerUp(Xpos, Ypos, Zpos, 1);
			}
			if (type == 8)
			{
				mEntityBuilder->createPowerUp(Xpos, Ypos, Zpos, 2);
			}
			if (type == 9)
			{
				mEntityBuilder->createPowerUp(Xpos, Ypos, Zpos, 3);
			}
			if (type == 11)
			{
				mEntityBuilder->createEndWall(Xpos, Ypos, Zpos, true);
			}
			if (type == 12)
			{
				mEntityBuilder->createPortal(Xpos, Ypos, Zpos);
			}
			if (type == 13){
				mEntityBuilder->createFloor(Xpos, Ypos, Zpos);

			}
		}
		loadMap(std::to_string(mLevel) + ".map");
		m_Timer = GameBoi::Timer::Instance();
		mSoundManager = new SoundManager();

		std::vector<std::string> faces;


		if (mLevel == 1){
			faces.push_back("../Resources/skybox/space/red/bkg1_right1.png");
			faces.push_back("../Resources/skybox/space/red/bkg1_left2.png");
			faces.push_back("../Resources/skybox/space/red/bkg1_top3.png");
			faces.push_back("../Resources/skybox/space/red/bkg1_bottom4.png");
			faces.push_back("../Resources/skybox/space/red/bkg1_front5.png");
			faces.push_back("../Resources/skybox/space/red/bkg1_back6.png");
		}
		else if (mLevel == 2){
			faces.push_back("../Resources/skybox/desert/right.jpeg");
			faces.push_back("../Resources/skybox/desert/left.jpeg");
			faces.push_back("../Resources/skybox/desert/up.jpeg");
			faces.push_back("../Resources/skybox/desert/down.jpeg");
			faces.push_back("../Resources/skybox/desert/front.jpeg");
			faces.push_back("../Resources/skybox/desert/back.jpeg");
		}
		else if (mLevel == 3){
			faces.push_back("../Resources/skybox/forest/right.jpeg");
			faces.push_back("../Resources/skybox/forest/left.jpeg");
			faces.push_back("../Resources/skybox/forest/up.jpeg");
			faces.push_back("../Resources/skybox/forest/down.jpeg");
			faces.push_back("../Resources/skybox/forest/front.jpeg");
			faces.push_back("../Resources/skybox/forest/back.jpeg");
		}


		mSkybox = new GameBoi::graphics::Skybox(faces);
		mSkyBoxShader->setInt("skybox", 0);

	}
	else
		init("1");
}

int GameWorld::getScore(void){
	return playerScore;
}
void GameWorld::setScore(int score){
		playerScore = score;
}

void GameWorld::saveGame()
{
	SaveGame save;
	std::ofstream ofs("lastSession.data", std::ios::binary);

	for (auto i : mEntityList)
	{
		save.Xpos = i->getPosition()->x;
		save.Ypos = i->getPosition()->y;
		save.Zpos = i->getPosition()->z;
		save.type = i->getType();

		if (i->getType() == E_ENTITY_TYPE::PLAYER)
		{
			auto moveComponent = dynamic_cast<PlayerMovementComponent *>(i->getComponent(E_COMPONENT_TYPE::COMPONENT_MOVEMENT));

			save.speed = moveComponent->getSpeed();
			save.range = i->getBombRange();
			save.lives = i->getLives();
			save.level = mLevel;
			save.isAlive = i->isAlive();
			save.score = playerScore;
		}
		if (i->getType() == E_ENTITY_TYPE::ENEMY)
		{
			auto moveComponent = dynamic_cast<EnemyMovementComponent *>(i->getComponent(E_COMPONENT_TYPE::COMPONENT_MOVEMENT));

			save.xDir = moveComponent->getXDir();
			save.zDir = moveComponent->getZDir();
			save.isAlive = i->isAlive();
		}
		if (i->getType() == E_ENTITY_TYPE::BOMB)
		{
			auto timer = dynamic_cast<TimerComponent *>(i->getComponent(E_COMPONENT_TYPE::COMPONENT_TIMER));
			save.elapsedTime = timer->getElapsed();
		}
		ofs.write((char *)&save, sizeof(save));
	}
}

void GameWorld::init(const std::string level)
{

	mLevel = stoi(level);
	loadMap(level + ".map");
	mEntityBuilder->setLevel(std::to_string(mLevel));
	spawnStartEntities();
	m_Timer = GameBoi::Timer::Instance();
	// auto config = Config::getInstance();
	mSoundManager = new SoundManager();  

	std::vector<std::string> faces;

	if (level == "1"){
		faces.push_back("../Resources/skybox/space/red/bkg1_right1.png");
		faces.push_back("../Resources/skybox/space/red/bkg1_left2.png");
		faces.push_back("../Resources/skybox/space/red/bkg1_top3.png");
		faces.push_back("../Resources/skybox/space/red/bkg1_bottom4.png");
		faces.push_back("../Resources/skybox/space/red/bkg1_front5.png");
		faces.push_back("../Resources/skybox/space/red/bkg1_back6.png");
	}
	else if (level == "2"){
		faces.push_back("../Resources/skybox/desert/right.jpeg");
		faces.push_back("../Resources/skybox/desert/left.jpeg");
		faces.push_back("../Resources/skybox/desert/up.jpeg");
		faces.push_back("../Resources/skybox/desert/down.jpeg");
		faces.push_back("../Resources/skybox/desert/front.jpeg");
		faces.push_back("../Resources/skybox/desert/back.jpeg");
	}
	else if (level == "3"){
		faces.push_back("../Resources/skybox/forest/right.jpeg");
		faces.push_back("../Resources/skybox/forest/left.jpeg");
		faces.push_back("../Resources/skybox/forest/up.jpeg");
		faces.push_back("../Resources/skybox/forest/down.jpeg");
		faces.push_back("../Resources/skybox/forest/front.jpeg");
		faces.push_back("../Resources/skybox/forest/back.jpeg");
	}


	mSkybox = new GameBoi::graphics::Skybox(faces);
	mSkyBoxShader->setInt("skybox", 0);

}

void GameWorld::playSound(E_SOUNDS sound ,bool loop)
{
	mSoundManager->play(sound, loop);
	return;
}

void GameWorld::stopAllSound(void){
	mSoundManager->stopAllSound();
}


float GameWorld::getScaling(void)
{
	return mScaling;
}

std::vector<std::vector<int>> GameWorld::getMap(void) const
{
	return this->m_map;
}

Camera *GameWorld::getCamera()
{
	return m_camera;
}

void GameWorld::nextLevel(void)
{
	saveGame();
	if (mLevel == 1)
	{
		nextState = STATE_LEVEL_2;
		playerScore += 10000;
	}
	else if (mLevel == 2)
	{
		nextState = STATE_LEVEL_3;
		playerScore += 15000;

	}
	else if (mLevel == 3)
	{
		playerScore += 10000;
		nextState = GAME_WON;
	}
}

void GameWorld::changeState(GAME_STATE state)
{
	nextState = state;
}

GAME_STATE GameWorld::getCurrentState(void)
{
	return nextState;
}

std::vector<GameEntity *> *GameWorld::getEntityList()
{
	return &mEntityList;
}

void GameWorld::addEntity(GameEntity *entity)
{
	mEntityCount++;
	entity->setId(mEntityCount);
	mEntityList.push_back(entity);
}

GameBoi::graphics::Window *GameWorld::getWindow()
{
	return m_window;
}

float GameWorld::getDeltaTime(void)
{
	return m_Timer->DeltaTime();
}

Builder *GameWorld::getBuilder(void)
{
	return this->mEntityBuilder;
}

void GameWorld::respawnPlayer()
{
	int lives = m_Player->getLives();
	lives--;
	if (lives <= 0)
	{
		nextState = GAME_STATE::GAME_OVER;
		std::cout << "GAME OVER MOTHERFUCKER!" << std::endl;
	}
	else
	{
		m_Player->setLives(lives);
		m_Player->setPosition(m_StartingBlock->getPosition()->x, m_StartingBlock->getPosition()->y, m_StartingBlock->getPosition()->z);
	}
};

unsigned int GameWorld::getEnemiesLeft(void){
	return mCurrentEnemies;
}

void GameWorld::setEnemiesLeft(unsigned int enemies){
	mCurrentEnemies = enemies;
}


void GameWorld::cleanUp(void)
{
	for (std::vector<GameEntity *>::iterator it = mEntityList.begin(); it != mEntityList.end(); /*it++*/)
	{
		GameEntity *entity = (*it);

		if (!entity->isAlive())
		{

			E_ENTITY_TYPE type = entity->getType();
			AnimationComponent *animation = dynamic_cast<AnimationComponent *>(entity->getComponent(E_COMPONENT_TYPE::COMPONENT_ANIMATION));
			GraphicsComponent *graphics = dynamic_cast<GraphicsComponent *>(entity->getComponent(E_COMPONENT_TYPE::COMPONENT_GRAPHICS));

			if (animation != NULL)
			{
				if (animation->getCurrentAnimation() == E_ANIMATION_TYPE::DEATH)
				{
					if (animation->isCurrentAnimationComplete())
					{

						if (type == PLAYER)
						{
							animation->setAnimationMode(IDLE);
							playerScore -= 500;
							respawnPlayer();
						}
						else
						{
							if (type == CRATE || type == ENEMY)
							{
								entity->randomDrop();
								if (type == CRATE)
									playerScore += 100;
								else{
									mCurrentEnemies--;
									playerScore += 1000 * mLevel;
								}

							}
							if (type == ENDGAMEWALL)
							{
								entity->DropEndGame();
								playerScore += 1000 * mLevel;

							}
							if (type == BOMB)
								m_Player->dropBombCount();
							delete ((*it));
							it = mEntityList.erase(it);
							return;
						}
					}
					else
					{
						animation->update();
						graphics->update();
					}
				}
				else
				{
					animation->setAnimationMode(E_ANIMATION_TYPE::DEATH);
				}
				it++;
			}
			else
			{
				if (type == CRATE || type == ENEMY)
				{
					entity->randomDrop();
					if (type == CRATE)
						playerScore += 100;
					else
					{
						mCurrentEnemies--;
						playerScore += 1000 * mLevel;
					}
				}
				if (type == ENDGAMEWALL)
				{
					entity->DropEndGame();
					playerScore += 1000 * mLevel;
				}
				if (type == BOMB)
					m_Player->dropBombCount();
				delete (*it);
				it = mEntityList.erase(it);
			}
		}
		else
			it++;
	}
	return;
}
