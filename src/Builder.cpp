#include "Builder.hpp"
#include "GameEntity.hpp"
#include "Component.hpp"
#include "GameWorld.hpp"

enum E_ANIMATION_TYPE;

void Builder::setLevel(std::string level){
	mLevel = level;
}

Builder::Builder()
{
	
}

Builder::~Builder(void){
	return;
}

Builder::Builder(const Builder &src){
	static_cast<void>(src);
	return;
}

Builder &Builder::operator=(const Builder &rhs){
	static_cast<void>(rhs);
	return *this;
}

Builder::Builder(GameWorld *world)
{
	m_world = world;
	mLevel = m_world->getLevel();
}

void Builder::setGameScale(float scale){
	mGameScale = scale;
}

void Builder::createFloor(double x, double y, double z)
{
	GameEntity *floor = new GameEntity(m_world, false);
	floor->setPosition(glm::vec3(x,y,z));
	floor->setType(E_ENTITY_TYPE::FLOOR);

	
	// Add a graphics component
	GraphicsComponent *graphics = new GraphicsComponent(floor);
	graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel(mLevel+"-model_floor"), glm::vec3(0.95f,1.0f,0.95f), 0, glm::vec3(1.0f, 0.1f, 1.0f));

	floor->addComponent(graphics);
	m_world->addEntity(floor);
}                                                                                   

void Builder::createWall(double x, double y, double z, bool canDestroy)
{
	

	GameEntity *wall = new GameEntity(m_world, canDestroy);
	wall->setType(E_ENTITY_TYPE::WALL);
	wall->setPosition(glm::vec3(x,y,z));
	
	// Add a graphics component
	GraphicsComponent *graphics = new GraphicsComponent(wall);
	if (!canDestroy){
		std::cout<< "MAKING WALL" << std::endl;
		graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel(mLevel+"-model_wall"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.0f,1.0f,1.0f));
	}else{
		std::cout<< "MAKING CRATE" << std::endl;
		graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel(mLevel+"-model_crate"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.0f,1.0f,1.0f));
		wall->setType(E_ENTITY_TYPE::CRATE);
	}

	// Add a collider component
	ColliderComponent *collider = new ColliderComponent(wall);
	collider->create(graphics->getSize().x * mGameScale, graphics->getSize().y *mGameScale);

	//add animation component
	AnimationComponent *animation = new AnimationComponent(wall);
	animation->loadModel(m_resources->getModel(mLevel + "-model_wall_die1"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel(mLevel + "-model_wall_die2"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel(mLevel + "-model_wall_die3"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel(mLevel + "-model_wall_die4"), E_ANIMATION_TYPE::DEATH);

	// set animation times
	//run , idle, death, ability
	animation->setInterval(0.0f, 0.0f, 0.06f, 0.0f);
	animation->setAnimationMode(ANIMATION_NONE);
	animation->setScaleAnimation(0.0f, E_ANIMATION_TYPE::ANIMATION_NONE);


	wall->addComponent(graphics);
	wall->addComponent(animation);

	wall->addComponent(collider);
	m_world->addEntity(wall);	
}

void Builder::createEndWall(double x, double y, double z, bool canDestroy)
{
	std::cout<< "MAKING ENDGAMEWALL" << std::endl;

	GameEntity *EndWall = new GameEntity(m_world, canDestroy);
	EndWall->setType(E_ENTITY_TYPE::ENDGAMEWALL);
	EndWall->setPosition(glm::vec3(x,y,z));
	
	// Add a graphics component
	GraphicsComponent *graphics = new GraphicsComponent(EndWall);
	graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel(mLevel+"-model_crate"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.0f,1.0f,1.0f));

	// Add a collider component
	ColliderComponent *collider = new ColliderComponent(EndWall);
	collider->create(graphics->getSize().x * mGameScale, graphics->getSize().y *mGameScale);

	//add animation component
	AnimationComponent *animation = new AnimationComponent(EndWall);
	animation->loadModel(m_resources->getModel(mLevel+"-model_wall_die1"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel(mLevel+"-model_wall_die2"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel(mLevel+"-model_wall_die3"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel(mLevel+"-model_wall_die4"), E_ANIMATION_TYPE::DEATH);

	// set animation times
	//run , idle, death, ability
	animation->setInterval(0.0f, 0.0f, 0.06f, 0.0f);
	animation->setAnimationMode(ANIMATION_NONE);
	animation->setScaleAnimation(0.0f, E_ANIMATION_TYPE::ANIMATION_NONE);


	EndWall->addComponent(graphics);
	EndWall->addComponent(animation);

	EndWall->addComponent(collider);
	m_world->addEntity(EndWall);	
}

void Builder::createPortal(double x, double y, double z)
{
	GameEntity *portal = new GameEntity(m_world, false);
	portal->setType(E_ENTITY_TYPE::PORTAL);
	portal->setPosition(glm::vec3(x,y,z));
	
	// Add a graphics component
	GraphicsComponent *graphics = new GraphicsComponent(portal);
	graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel(mLevel+"-model_portal1"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.0f,1.0f,1.0f));

	// Add a collider component
	ColliderComponent *collider = new ColliderComponent(portal);
	collider->create(graphics->getSize().x * mGameScale, graphics->getSize().y *mGameScale);

	//add animation component
	AnimationComponent *animation = new AnimationComponent(portal);
	animation->loadModel(m_resources->getModel(mLevel+"-model_portal1"), E_ANIMATION_TYPE::IDLE);
	animation->loadModel(m_resources->getModel(mLevel+"-model_portal2"), E_ANIMATION_TYPE::IDLE);
	animation->loadModel(m_resources->getModel(mLevel+"-model_portal3"), E_ANIMATION_TYPE::IDLE);
	animation->loadModel(m_resources->getModel(mLevel+"-model_portal4"), E_ANIMATION_TYPE::IDLE);

	// set animation times
	// run , idle, death, ability
	animation->setInterval(0.0f, 0.1f, 0.0f, 0.0f);
	animation->setAnimationMode(IDLE);
	animation->setScaleAnimation(0.0f, E_ANIMATION_TYPE::ANIMATION_NONE);


	portal->addComponent(graphics);
	portal->addComponent(animation);
	portal->addComponent(collider);
	m_world->addEntity(portal);	

}

void Builder::createExplosion(double x, double y, double z){
	GameEntity *explosion = new GameEntity(m_world, false);
	explosion->setType(E_ENTITY_TYPE::EXPLOSION);
	explosion->setPosition(glm::vec3(x,y,z));

	// Add a graphics component
	GraphicsComponent *graphics = new GraphicsComponent(explosion);
	graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("model_explosion"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.0f, 1.0f, 1.0f));

	//add a timer component
	TimerComponent *timer = new TimerComponent(explosion);

	explosion->addComponent(timer);
	explosion->addComponent(graphics);
	m_world->addEntity(explosion);	

}

GameEntity *Builder::createPlayer(double x, double y, double z)
{
	std::cout<< "MAKING PLAYER" << std::endl;
	GameEntity *player = new GameEntity(m_world, true);
	player->setPosition(glm::vec3(x,y,z));
	player->setType(E_ENTITY_TYPE::PLAYER);
	player->setLives(3);
	player->setBombRange(1.0f);

	
	// Add a graphics component
	GraphicsComponent *graphics = new GraphicsComponent(player);
	graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("model_player_stand1"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(0.8f, 0.8f, 0.8f));

	//add animation component
	AnimationComponent *animation = new AnimationComponent(player);
	animation->loadModel(m_resources->getModel("model_player_stand1"), E_ANIMATION_TYPE::IDLE);
	animation->loadModel(m_resources->getModel("model_player_stand2"), E_ANIMATION_TYPE::IDLE);
	animation->loadModel(m_resources->getModel("model_player_stand3"), E_ANIMATION_TYPE::IDLE);
	animation->loadModel(m_resources->getModel("model_player_stand2"), E_ANIMATION_TYPE::IDLE);

	animation->loadModel(m_resources->getModel("model_player_run1"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel("model_player_run2"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel("model_player_run3"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel("model_player_run2"), E_ANIMATION_TYPE::RUN);


	animation->loadModel(m_resources->getModel("model_player_place1"), E_ANIMATION_TYPE::ABILITY);
	animation->loadModel(m_resources->getModel("model_player_place2"), E_ANIMATION_TYPE::ABILITY);
	animation->loadModel(m_resources->getModel("model_player_place1"), E_ANIMATION_TYPE::ABILITY);
	animation->loadModel(m_resources->getModel("model_player_place2"), E_ANIMATION_TYPE::ABILITY);
	animation->loadModel(m_resources->getModel("model_player_place3"), E_ANIMATION_TYPE::ABILITY);
	animation->loadModel(m_resources->getModel("model_player_place3"), E_ANIMATION_TYPE::ABILITY);

	animation->loadModel(m_resources->getModel("model_player_die1"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel("model_player_die3"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel("model_player_die4"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel("model_player_die4"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel("model_player_stand1"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel("model_player_stand1"), E_ANIMATION_TYPE::DEATH);

	// set animation times
	//run , idle, death, ability
	animation->setInterval(0.10f, 0.25f, 0.15f, 0.060f);
	animation->setAnimationMode(IDLE);
	animation->setScaleAnimation(0.0f, E_ANIMATION_TYPE::ANIMATION_NONE);

	// Add a movement component
	PlayerMovementComponent *movement = new PlayerMovementComponent(player);
	movement->create(graphics, 3.0f);

	// Add a collider component
	ColliderComponent *collider = new ColliderComponent(player);
	collider->create(graphics->getSize().x* mGameScale, graphics->getSize().y *mGameScale);

	player->addComponent(graphics);
	player->addComponent(movement);
	player->addComponent(collider);
	player->addComponent(animation);

	m_world->addEntity(player);

	return player;
}

GameEntity * Builder::createEnemy(double x, double y, double z)
{
	std::cout<< "MAKING ENEMY" << std::endl;
	GameEntity *enemy = new GameEntity(m_world, true);
	enemy->setPosition(glm::vec3(x,y,z));
	enemy->setType(E_ENTITY_TYPE::ENEMY);
	
	// Add a graphics component
	GraphicsComponent *graphics = new GraphicsComponent(enemy);
	graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel(mLevel +"-model_enemy"), glm::vec3(2.0f,2.0f,2.0f), 0, glm::vec3(1.0f, 1.0f, 1.0f));

	// Add a movement component
	EnemyMovementComponent *movement = new EnemyMovementComponent(enemy);
	movement->create(graphics, 1.2f);

	// Add a collider component
	ColliderComponent *collider = new ColliderComponent(enemy);
	collider->create(graphics->getSize().x *mGameScale, graphics->getSize().y*mGameScale);

	//add animation component
	AnimationComponent *animation = new AnimationComponent(enemy);

	animation->loadModel(m_resources->getModel(mLevel + "-model_monster_run1"), E_ANIMATION_TYPE::IDLE);


	animation->loadModel(m_resources->getModel(mLevel + "-model_monster_run1"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel(mLevel + "-model_monster_run2"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel(mLevel + "-model_monster_run3"), E_ANIMATION_TYPE::RUN);
	animation->loadModel(m_resources->getModel(mLevel +"-model_monster_run2"), E_ANIMATION_TYPE::RUN);


	animation->loadModel(m_resources->getModel(mLevel + "-model_monster_die1"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel(mLevel + "-model_monster_die2"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel(mLevel + "-model_monster_die3"), E_ANIMATION_TYPE::DEATH);
	animation->loadModel(m_resources->getModel(mLevel + "-model_monster_die4"), E_ANIMATION_TYPE::DEATH);

	//run , idle, death, ability
	animation->setInterval(0.10f, 0.0f, 0.10f, 0.0f);
	animation->setAnimationMode(RUN);
	animation->setScaleAnimation(0.0f, E_ANIMATION_TYPE::ANIMATION_NONE);


	enemy->addComponent(graphics);
	enemy->addComponent(movement);
	enemy->addComponent(collider);
	enemy->addComponent(animation);

	m_world->setEnemiesLeft(m_world->getEnemiesLeft() + 1);

	m_world->addEntity(enemy);
	return enemy;

}

GameEntity * Builder::createBomb(double x, double y, double z)
{
	std::cout<< "MAKING BOMB" << std::endl;

	GameEntity *bomb = new GameEntity(m_world, false);
	bomb->setPosition(glm::vec3(x,y,z));
	bomb->setType(E_ENTITY_TYPE::BOMB);
	bomb->setBombRange(m_world->getPlayer()->getBombRange() * mGameScale);

	// Add a graphics component
	GraphicsComponent *graphics = new GraphicsComponent(bomb);
	graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("model_bomb"), glm::vec3(0.25f,0.25f,0.25f), 0, glm::vec3(1.0f, 1.0f, 1.0f));

	// Add a collider component
	ColliderComponent *collider = new ColliderComponent(bomb);
	collider->create(graphics->getSize().x*mGameScale, graphics->getSize().y*mGameScale);

	//add animation component
	AnimationComponent *animation = new AnimationComponent(bomb);

	animation->loadModel(m_resources->getModel("model_bomb"), E_ANIMATION_TYPE::IDLE);


	//run , idle, death, ability
	animation->setInterval(0.0f, 0.99f, 0.0f, 0.0f);
	animation->setAnimationMode(IDLE);
	animation->setScaleAnimation(1.0f, E_ANIMATION_TYPE::IDLE);

	//add a timer component
	TimerComponent *timer = new TimerComponent(bomb);

	bomb->addComponent(timer);
	bomb->addComponent(graphics);
	bomb->addComponent(collider);
	bomb->addComponent(animation);
	m_world->addEntity(bomb);
	return bomb;
}


GameEntity *Builder::createStartingGate(double x, double y, double z){
	std::cout<< "MAKING STARTGATE" << std::endl;

	GameEntity *wall = new GameEntity(m_world, false);
	wall->setType(E_ENTITY_TYPE::INVISIBLEWALL);
	wall->setPosition(glm::vec3(x,y,z));
	
	GraphicsComponent *graphics = new GraphicsComponent(wall);
	graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel(mLevel+ "-model_wall"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.05f, 1.05f, 1.05f));


	// Add a collider component
	ColliderComponent *collider = new ColliderComponent(wall);
	collider->create(graphics->getSize().x*mGameScale, graphics->getSize().y*mGameScale);

	wall->addComponent(collider);
	// wall->addComponent(graphics);

	m_world->addEntity(wall);
	return wall;
}


void  Builder::createPowerUp(double x, double y, double z, int type){
	std::cout<< "MAKING POWERUP" << std::endl;

	GameEntity *powerup = new GameEntity(m_world, false);

	powerup->setPosition(glm::vec3(floor(x),y,floor(z)));

	GraphicsComponent *graphics = new GraphicsComponent(powerup);

	if (type == 1){
		powerup->setType(SPEED_UP);
		graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("powerup_speed"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.0f, 1.0f, 1.0f));
	} else if (type == 2){
		powerup->setType(LIFE_UP);
		graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("powerup_life"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.0f, 1.0f, 1.0f));
	} else if (type == 3){
		powerup->setType(BOMB_UP);
		graphics->create(m_resources->getShader("shader_basic"), m_resources->getModel("powerup_bomb"), glm::vec3(1.0f,1.0f,1.0f), 0, glm::vec3(1.0f, 1.0f, 1.0f));
	}


	// Add a collider component
	ColliderComponent *collider = new ColliderComponent(powerup);
	collider->create(graphics->getSize().x*mGameScale, graphics->getSize().y*mGameScale);

	powerup->addComponent(collider);
	powerup->addComponent(graphics);


	m_world->addEntity(powerup);
}


void Builder::create(GameWorld *world, ResourceManager *resources)
{
	m_world = world;
	m_resources = resources;
}
