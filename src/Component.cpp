#include "Component.hpp"
#include "GameEntity.hpp"
#include "GameWorld.hpp"
#include "../GameEngine/graphics/includes/Models.hpp"
#include "Config.hpp"
#include <iostream>
// #include "Sound.hpp"

Component::Component()
{

}

Component::~Component(void)
{

}

Component::Component(const Component &src)
{
	static_cast<void>(src);
	return;
}
Component &Component::operator=(const Component &rhs)
{
	static_cast<void>(rhs);
	return *this;
}

Component::Component(GameEntity *owner)
{
	if (owner == NULL){
		std::cout << "Component::Component - owner passed is NULL" << std::endl;
	}
	m_owner = owner;

	mType = E_COMPONENT_TYPE::COMPONENT_DEFAULT;
}

E_COMPONENT_TYPE Component::getType()
{
	return mType;
}

GameEntity *Component::getOwner()
{
	return m_owner;
}

// ============================================================= GRAPHICS COMPONENT ========================================================//


GraphicsComponent::GraphicsComponent(GameEntity * owner)
:Component(owner)
{
	mType = E_COMPONENT_TYPE::COMPONENT_GRAPHICS;

	m_shader = NULL; 
	m_model = NULL;
}

void GraphicsComponent::create(GameBoi::graphics::Shader *shader, GameBoi::graphics::Model *model, glm::vec3 scale, float rotation, glm::vec3 size)
{
	m_shader = shader; 
	m_model = model;
	mScale = scale; 
	mRotation = rotation;
	mSize = size;
	mOriginalScale = scale;
	m_shader->use();
}

void GraphicsComponent::setSize(glm::vec3 size){
	mSize = size;
}

void GraphicsComponent::multiplyScale(float scale){
		std::cout << "multiply scale" << std::endl;
		mScale.x += mOriginalScale.x* scale;
		mScale.y += mOriginalScale.y* scale;
		mScale.z += mOriginalScale.z* scale;
}


void GraphicsComponent::update() 
{
	
}

glm::vec3 &GraphicsComponent::getSize()
{
	return mSize;
}

void GraphicsComponent::setRotation(float rot)
{
	mRotation = rot;
}

GameBoi::graphics::Model *GraphicsComponent::getModel()
{
	return m_model;
}

void GraphicsComponent::render() 
{
	Camera *camera = m_owner->getWorld()->getCamera();
	m_shader->use();
	m_shader->setMat4("projection", glm::perspective(glm::radians(camera->Zoom), (float)SCREEN_WIDTH/ (float)SCREEN_HEIGHT, 0.1f, 500.0f));
	m_shader->setMat4("view", camera->GetViewMatrix());
	
	// calculate the model matrix for each object and pass it to shader before drawing
	m_model->NewPostionAndScale(*m_owner->getPosition(),mScale, mRotation);
	m_model->DrawAndSet(*m_shader, "model");  


}

void GraphicsComponent::setModel(GameBoi::graphics::Model *_model){
	m_model = _model;
}

GraphicsComponent::GraphicsComponent()
{

}
GraphicsComponent::GraphicsComponent(const GraphicsComponent &src)
{
	static_cast<void>(src);
	return;
}
GraphicsComponent &GraphicsComponent::operator=(const GraphicsComponent &rhs)
{  
	static_cast<void>(rhs);
	return *this;
}

GraphicsComponent::~GraphicsComponent(void)
{

}


// ============================================================= PLAYER MOVEMENT COMPONENT ========================================================//

PlayerMovementComponent::PlayerMovementComponent(const PlayerMovementComponent & _gComp)
{
	static_cast<void>(_gComp);
	return;
}

PlayerMovementComponent & PlayerMovementComponent::operator=(const PlayerMovementComponent & _rhs)
{
	static_cast<void>(_rhs);
	return *this;
}
PlayerMovementComponent::PlayerMovementComponent(void)
{

}

PlayerMovementComponent::~PlayerMovementComponent(void)
{

}


PlayerMovementComponent::PlayerMovementComponent(GameEntity *owner)
:Component(owner)
{
	mType = E_COMPONENT_TYPE::COMPONENT_MOVEMENT;
}

void PlayerMovementComponent::create(GraphicsComponent *graphics, float speed)
{
	m_graphics = graphics;
	mSpeed = speed;
}

E_ANIMATION_TYPE PlayerMovementComponent::getMoveState(void){
	return mState;
};


bool PlayerMovementComponent::checkBombPlacement(float x, float z, GameEntity *b) 
{
	float	scale = m_owner->getWorld()->getScaling();
	float 	remX = fmod(x,scale);
	float	remZ = fmod(z,scale);
	float xpos = m_owner->getPosition()->x  - remX + scale/2.0f;
	float zpos = m_owner->getPosition()->y  - remZ + scale/2.0f;

	if (xpos == b->getPosition()->x && zpos == b->getPosition()->z){
		return true;
	}
	return false;
}
	
void PlayerMovementComponent::update()
{
	auto window = this->m_owner->getWorld()->getWindow();
	auto player = this->m_owner;
	auto world = player->getWorld();
	auto playerCollider = dynamic_cast<ColliderComponent*>(player->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));
	float DeltaTime = player->getWorld()->getDeltaTime();
	float speed = mSpeed * world->getScaling();
	float distance = DeltaTime * speed;
	Config *config = Config::getInstance();

	if (window->isKeyPressed(config->KEY_LEFT)) {
		m_graphics->setRotation(90);
		player->setPosition(player->getPosition()->x + distance, player->getPosition()->y, player->getPosition()->z);
		playerCollider->setCollision();
		handleCollision(1.0f,0.0f,distance);
	}
	else if (window->isKeyPressed(config->KEY_RIGHT)) {
		m_graphics->setRotation(270);
		player->setPosition(player->getPosition()->x - distance, player->getPosition()->y, player->getPosition()->z);
		playerCollider->setCollision();
		handleCollision(-1.0f,0.0f,distance);

	}
	else if (window->isKeyPressed(config->KEY_UP)) {
		m_graphics->setRotation(0);
		player->setPosition(player->getPosition()->x, player->getPosition()->y, player->getPosition()->z + distance);
		playerCollider->setCollision();
		handleCollision(0.0f,1.0f,distance);

	}
	else if (window->isKeyPressed(config->KEY_DOWN)) {
		m_graphics->setRotation(180);
		player->setPosition(player->getPosition()->x, player->getPosition()->y, player->getPosition()->z - distance);
		playerCollider->setCollision();
		handleCollision(0.0f,-1.0f,distance);

	}
	else if (window->isKeyPressed(config->KEY_DROPBOMB)) {
		bool canPlace = true;
		playerCollider->setContainingBlock();
		float blockX = playerCollider->getContainerBlockX();
		float blockZ = playerCollider->getContainerBlockZ();

		for (auto i : *(m_owner->getWorld()->getEntityList()))
		{
			if (i->getType() == E_ENTITY_TYPE::BOMB){
				auto bombCollider = dynamic_cast<ColliderComponent*>(i->getComponent(COMPONENT_COLLIDER));
				if (bombCollider->isWithinBlock(blockX, blockZ)){
					canPlace = false;
				}
			}
			if (i->getType() == E_ENTITY_TYPE::ENEMY){
				auto enemyCollider = dynamic_cast<ColliderComponent*>(i->getComponent(COMPONENT_COLLIDER));
				if (enemyCollider->isWithinBlock(blockX, blockZ)){
					canPlace = false;
				}
			} 
		}
		if (canPlace){
			mState = E_ANIMATION_TYPE::ABILITY;
			
		}
	} 
	else{
		mState = E_ANIMATION_TYPE::IDLE;
	}

}

void PlayerMovementComponent::handleCollision(float xDir, float zDir, float distance){
	auto player = this->m_owner;
	auto playerCollider = dynamic_cast<ColliderComponent*>(player->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));
	auto victim = playerCollider->checkCollision();

	float xDistance = distance * xDir;
	float zDistance = distance * zDir;

	if (victim != NULL){
		E_ENTITY_TYPE type = victim->getType();
		if (type == E_ENTITY_TYPE::WALL || type == E_ENTITY_TYPE::ENDGAMEWALL || type == E_ENTITY_TYPE::CRATE){

	 		mState = E_ANIMATION_TYPE::IDLE;
			playerCollider->clearCollision();
			player->setPosition(player->getPosition()->x - xDistance, player->getPosition()->y, player->getPosition()->z - zDistance);
		}
		else if (type == E_ENTITY_TYPE::SPEED_UP){
			mState = E_ANIMATION_TYPE::RUN;
			mSpeed+=0.5;
			victim->kill();
			playerCollider->clearCollision();
		}
		else if (type == E_ENTITY_TYPE::BOMB_UP){
			mState = E_ANIMATION_TYPE::RUN;
			player->setBombRange(player->getBombRange() + 1.0f);
			victim->kill();
			playerCollider->clearCollision();
		}
		else if (type == E_ENTITY_TYPE::LIFE_UP){
			mState = E_ANIMATION_TYPE::RUN;
			m_owner->setLives(m_owner->getLives() + 1);
			victim->kill();
			playerCollider->clearCollision();
		}
		else if (type == E_ENTITY_TYPE::PORTAL){
			if (m_owner->getWorld()->getEnemiesLeft() == 0)
				m_owner->getWorld()->nextLevel();
		}
	}else{
		mState = E_ANIMATION_TYPE::RUN;
	}

}


float PlayerMovementComponent::getSpeed()
{
	return mSpeed;
}
void PlayerMovementComponent::setSpeed(float speed)
{
	mSpeed = speed;
	if (mSpeed > 5.0f){
		mSpeed = 5.0f;
	}
}

// ============================================================= ENEMY MOVEMENT COMPONENT ========================================================//

EnemyMovementComponent::EnemyMovementComponent(const EnemyMovementComponent & _gComp)
{
	static_cast<void>(_gComp);
	return;
}
EnemyMovementComponent & EnemyMovementComponent::operator=(const EnemyMovementComponent & _rhs)
{
	static_cast<void>(_rhs);
	return *this;
}
EnemyMovementComponent::EnemyMovementComponent(void)
{

}
EnemyMovementComponent::~EnemyMovementComponent()
{

}

EnemyMovementComponent::EnemyMovementComponent(GameEntity *owner)
:Component(owner)
{
	mType = E_COMPONENT_TYPE::COMPONENT_MOVEMENT;
}

void EnemyMovementComponent::create(GraphicsComponent *graphics, float speed)
{
	m_graphics = graphics;
	mSpeed = speed;
	xDirection = 1;
	zDirection = 0;
}

E_ANIMATION_TYPE EnemyMovementComponent::getMoveState(void){
	return mState;
};

void EnemyMovementComponent::handleCollision(float xDir, float zDir, float distance){
	auto enemy = this->m_owner;
	auto enemyCollider = dynamic_cast<ColliderComponent*>(enemy->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));
	auto victim = enemyCollider->checkCollision();
	
	float xDistance = distance * xDir;
	float zDistance = distance * zDir;

	if (victim != NULL){
		E_ENTITY_TYPE type = victim->getType();
		if (type == E_ENTITY_TYPE::WALL || type == E_ENTITY_TYPE::INVISIBLEWALL || type == E_ENTITY_TYPE::ENDGAMEWALL || type == E_ENTITY_TYPE::CRATE){
			enemy->setPosition(enemy->getPosition()->x - xDistance, enemy->getPosition()->y, enemy->getPosition()->z - zDistance);
			this->xDirection = 0;
			this->zDirection = 0;
		}
		if (type == E_ENTITY_TYPE::ENEMY){
			enemy->setPosition(enemy->getPosition()->x - xDistance, enemy->getPosition()->y, enemy->getPosition()->z - zDistance);
			this->xDirection = 0;
			this->zDirection = 0;
		}
		if (type == E_ENTITY_TYPE::BOMB){
			enemy->setPosition(enemy->getPosition()->x - xDistance, enemy->getPosition()->y, enemy->getPosition()->z - zDistance);
			this->xDirection = 0;
			this->zDirection = 0;
		}
		if (type == E_ENTITY_TYPE::PLAYER){
			enemy->setPosition(enemy->getPosition()->x - xDistance, enemy->getPosition()->y, enemy->getPosition()->z - zDistance);
			victim->kill();
			this->xDirection = 0;
			this->zDirection = 0;
		}
		if (type == E_ENTITY_TYPE::PORTAL){
			enemy->setPosition(enemy->getPosition()->x - xDistance, enemy->getPosition()->y, enemy->getPosition()->z - zDistance);
			this->xDirection = 0;
			this->zDirection = 0;
		}
		
		enemyCollider->clearCollision();
	}
	mState = E_ANIMATION_TYPE::RUN;
}

void EnemyMovementComponent::update()
{
	auto enemy = this->m_owner;
	auto enemyCollider = dynamic_cast<ColliderComponent*> (enemy->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));
	float DeltaTime = enemy->getWorld()->getDeltaTime();
	float speed = mSpeed * this->m_owner->getWorld()->getScaling();
	float distance = DeltaTime * speed;

	// east
	if (xDirection > 0 && zDirection == 0) {
		m_graphics->setRotation(90);
		enemy->setPosition(enemy->getPosition()->x + distance, enemy->getPosition()->y, enemy->getPosition()->z);
		enemyCollider->setCollision();
		handleCollision(1.0f,0.0f,distance);
	}
	// west
	else if (xDirection < 0 && zDirection == 0) {
		m_graphics->setRotation(270);
		enemy->setPosition(enemy->getPosition()->x - distance, enemy->getPosition()->y, enemy->getPosition()->z);
		enemyCollider->setCollision();
		handleCollision(-1.0f,0.0f,distance);

	}
	//north
	else if (xDirection == 0 && zDirection > 0) {
		m_graphics->setRotation(0);
		enemy->setPosition(enemy->getPosition()->x, enemy->getPosition()->y, enemy->getPosition()->z + distance);
		enemyCollider->setCollision();
		handleCollision(0.0f,1.0f,distance);

	}
	//south
	else if (xDirection == 0 && zDirection < 0) {
		m_graphics->setRotation(180);
		enemy->setPosition(enemy->getPosition()->x, enemy->getPosition()->y, enemy->getPosition()->z - distance);
		enemyCollider->setCollision();
		handleCollision(0.0f,-1.0f,distance);
	}
	//player needs a new direction
	else {
		enemyCollider->clearCollision();
		possibleMoves();
		randomDirection();
	}
}

void EnemyMovementComponent::possibleMoves(void){
	
	auto enemy = this->m_owner;
	auto enemyCollider = dynamic_cast<ColliderComponent*>(enemy->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));
	GameEntity *victim;
	glm::vec3 *originalPosition = enemy->getPosition();
	float originalX = originalPosition->x;
	float originalY = originalPosition->y;
	float originalZ = originalPosition->z;
	float scaling = 1.0f * this->m_owner->getWorld()->getScaling();


	mPossibleMoves.clear();


	//east
	enemy->setPosition(originalX + scaling, originalY, originalZ);
	enemyCollider->setCollision();
	victim = enemyCollider->checkCollision();
	if (victim != NULL){
		if (victim->getType() != E_ENTITY_TYPE::WALL && victim->getType() != E_ENTITY_TYPE::BOMB && victim->getType() != E_ENTITY_TYPE::ENEMY){
			mPossibleMoves.push_back(E_DIRECTIONS::EAST);
		}
	}else {
		mPossibleMoves.push_back(E_DIRECTIONS::EAST);
	}

	//west
	enemy->setPosition(originalX - scaling, originalY, originalZ);
	enemyCollider->setCollision();
	victim = enemyCollider->checkCollision();
	if (victim != NULL)
	{
		if (victim->getType() != E_ENTITY_TYPE::WALL && victim->getType() != E_ENTITY_TYPE::BOMB && victim->getType() != E_ENTITY_TYPE::ENEMY){
			mPossibleMoves.push_back(E_DIRECTIONS::WEST);
		}
	} else {
		mPossibleMoves.push_back(E_DIRECTIONS::WEST);
	}

	//south
	enemy->setPosition(originalX, originalY, originalZ - scaling);
	enemyCollider->setCollision();
	victim = enemyCollider->checkCollision();
	if (victim != NULL){
		if (victim->getType() != E_ENTITY_TYPE::WALL && victim->getType() != E_ENTITY_TYPE::BOMB && victim->getType() != E_ENTITY_TYPE::ENEMY){
			mPossibleMoves.push_back(E_DIRECTIONS::SOUTH);
		}
	}
	else {
		mPossibleMoves.push_back(E_DIRECTIONS::SOUTH);
	}

	//north
	enemy->setPosition(originalX , originalY, originalZ + scaling);
	enemyCollider->setCollision();
	victim = enemyCollider->checkCollision();
	if (victim != NULL){
		if (victim->getType() != E_ENTITY_TYPE::WALL && victim->getType() != E_ENTITY_TYPE::BOMB && victim->getType() != E_ENTITY_TYPE::ENEMY){
			mPossibleMoves.push_back(E_DIRECTIONS::NORTH);
		}
	} else {
		mPossibleMoves.push_back(E_DIRECTIONS::NORTH);
	}

	enemy->setPosition(originalX, originalY, originalZ);

	return;
}

void EnemyMovementComponent::randomDirection(void){
	int randomIndex;
	E_DIRECTIONS type;

	srand(time(NULL));
	if (mPossibleMoves.size() == 0){
		xDirection = 0;
		zDirection = 0;
		return;
	}

	randomIndex = static_cast<int>(rand() % mPossibleMoves.size());
	type = static_cast<E_DIRECTIONS>(mPossibleMoves[randomIndex]);

	switch(type){
		case NORTH:
			xDirection= 0;
			zDirection = 1;
			break;
		case EAST:
			xDirection = 1;
			zDirection = 0;
			break;
		case SOUTH:
			xDirection = 0;
			zDirection = -1;
			break;
		case WEST:
			xDirection = -1;
			zDirection = 0;
			break;
	}

};

float EnemyMovementComponent::getSpeed()
{
	return mSpeed;
}
void EnemyMovementComponent::setSpeed(float speed)
{
	mSpeed = speed;
}

int 	EnemyMovementComponent::getXDir(void){
	return xDirection;
}

int 	EnemyMovementComponent::getZDir(void){
	return zDirection;
}

void	EnemyMovementComponent::setDirection(int x, int z){
	xDirection = x;
	zDirection = z;
}

// ============================================================= COLLIDER COMPONENT ========================================================//


// ColliderComponent & ColliderComponent::operator=(const ColliderComponent & _rhs){

// 	static_cast<void>(_rhs);
// 	return *this;
// }

ColliderComponent::ColliderComponent(const ColliderComponent & _gComp)
{
	static_cast<void>(_gComp);
	return;
}
ColliderComponent & ColliderComponent::operator=(const ColliderComponent & _rhs)
{
	static_cast<void>(_rhs);
	return *this;
}
ColliderComponent::ColliderComponent(void)
{

}
ColliderComponent::~ColliderComponent()
{

}
	

ColliderComponent::ColliderComponent(GameEntity *owner)
:Component(owner)
{
	mType = E_COMPONENT_TYPE::COMPONENT_COLLIDER;
	m_Collision = NULL;
}

void ColliderComponent::create(float xSize, float ySize)
{
	mXSize = xSize; 
	mYSize = ySize;
}


void ColliderComponent::update()
{

}

float ColliderComponent::getXSize()
{
	return mXSize;
}

float ColliderComponent::getYSize()
{
	return mYSize;
}

float ColliderComponent::getContainerBlockX()
{
	return mContainingBlockX;
}

float ColliderComponent::getContainerBlockZ()
{
	return mContainingBlockZ;
}

bool DoBoxesIntersect(GameEntity *a, GameEntity *b) 
{

	auto aCollider = dynamic_cast<ColliderComponent*> (a->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));
	auto bCollider = dynamic_cast<ColliderComponent*> (b->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));

  return (abs(a->getPosition()->x - b->getPosition()->x) * 2 < (aCollider->getXSize()/1.2f + bCollider->getXSize())) &&
		 (abs(a->getPosition()->z - b->getPosition()->z) * 2 < (aCollider->getYSize()/1.2f + bCollider->getYSize()));
}

bool DoBoxesIntersectSoft(GameEntity *a, GameEntity *b) 
{

	auto aCollider = dynamic_cast<ColliderComponent*> (a->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));
	auto bCollider = dynamic_cast<ColliderComponent*> (b->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));

  return (abs(a->getPosition()->x - b->getPosition()->x) * 2 < (aCollider->getXSize()/1.9f + bCollider->getXSize())) &&
		 (abs(a->getPosition()->z - b->getPosition()->z) * 2 < (aCollider->getYSize()/1.9f + bCollider->getYSize()));
}


void ColliderComponent::setContainingBlock(void){
	auto world = m_owner->getWorld();
	auto colliderComponent = dynamic_cast<ColliderComponent*>(m_owner->getComponent(COMPONENT_COLLIDER));
	float scale = world->getScaling();
	float worldSize = world->getMapSize() * scale;
	float closestCenter = scale;
	// std::cout << "MAPS SCALE: " << scale << std::endl;
	float entityCenterX = m_owner->getPosition()->x - mXSize/2.0f;
	float entityCenterZ = m_owner->getPosition()->z  - mYSize/2.0f;

	for(float x = 0; x < worldSize; x += scale){
		for(float z = 0; z < worldSize; z += scale){
			if (colliderComponent->isWithinBlock(x, z)){
					// std::cout << "WITHIN BLOCK X: " << x<< " Z: " << z << std::endl;
				float distance = sqrt(pow(entityCenterX-(x-scale/2.0f),2.0f) + pow(entityCenterZ-(z-scale/2.0f),2.0f));
				if (distance < closestCenter){
					// std::cout << "CLOSEST BLOCK X: " << x<< " Z: " << z << std::endl;
					closestCenter = distance;
					mContainingBlockX = x;
					mContainingBlockZ = z;
				}
				
			}
		}
	}
	return;
}

bool ColliderComponent::isWithinBlock(float blockX, float blockZ){
	float gameUnit = m_owner->getWorld()->getScaling();
	float entityX = m_owner->getPosition()->x;
	float entityZ = m_owner->getPosition()->z;

	//directly on top of eachother
	if (entityX == blockX && entityZ == blockZ){
		return true;
	}

	if (entityX == blockX && entityZ < blockZ + gameUnit && blockZ < entityZ){
		// std::cout << "top side collision" << std::endl;
		return true;
	} else if (entityX == blockX && entityZ + mYSize > blockZ && blockZ > entityZ){
		// std::cout << "bottom side collision" << std::endl;
		return true;
	}

	if (entityZ == blockZ && entityX < blockX + gameUnit && blockX < entityX){
		// std::cout << "left side collision" << std::endl;
		return true;
	} else if (entityZ == blockZ && entityX + mXSize > blockX && blockX > entityX){
		// std::cout << "right side collision" << std::endl;
		return true;
	}

	// if left side of player overlaps right side of wall && right side doesnt overlap left side of wall
	if (entityX < blockX + gameUnit && entityX > blockX){
		// std::cout << "if left side of player overlaps right side of wall" << std::endl;
		if (entityZ > blockZ && entityZ < blockZ + gameUnit){
			// std::cout << "top left corner collision" << std::endl;
			return true;
		}else if (entityZ < blockZ && entityZ + mYSize > blockZ){
			// std::cout << "bottom left corner collision" << std::endl;
			return true;
		}
	}
	// if right side of player overlaps left side of wall
	if (entityX + mXSize > blockX && entityX < blockX){
		// std::cout << "if right side of player overlaps left side of wall" << std::endl;
		if (entityZ > blockZ && entityZ < blockZ + mYSize){
			// std::cout << "top right corner collision" << std::endl;
			return true;
		}else if (entityZ < blockZ && entityZ + mYSize > blockZ){
			// std::cout << "bottom right corner collision" << std::endl;
			return true;
		}
	}
	return false;
}

void ColliderComponent::clearCollision(void){
	m_Collision = NULL;
}

bool ColliderComponent::checkIntersection(GameEntity * _a, GameEntity * _b){
	return (DoBoxesIntersect(_a, _b));
}

void ColliderComponent::setCollision(void){
	bool collision = false;

	for (auto i : *(m_owner->getWorld()->getEntityList()))
	{
		if (i->getId() != m_owner->getId() && i->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER)){
			if ((m_owner->getType() == PLAYER && i->getType() == BOMB) || (i->getType() == INVISIBLEWALL && m_owner->getType() == PLAYER))
				continue;
			else if (m_owner->getType() == ENEMY){
				if (i->getType() == SPEED_UP || i->getType() == BOMB_UP || i->getType() == LIFE_UP){
					continue;
				}
			}


			if ((m_owner->getType() == PLAYER && i->getType() == ENEMY) || (m_owner->getType() == PLAYER && i->getType() == PORTAL) || (m_owner->getType() == ENEMY && i->getType() == PLAYER)){
				auto playerCollider = dynamic_cast<ColliderComponent*>(m_owner->getComponent(COMPONENT_COLLIDER));
				auto enemyCollider = dynamic_cast<ColliderComponent*>(i->getComponent(COMPONENT_COLLIDER));

				playerCollider->setContainingBlock();
				enemyCollider->setContainingBlock();

				if (playerCollider->getContainerBlockX() == enemyCollider->getContainerBlockX() &&
					playerCollider->getContainerBlockZ() == enemyCollider->getContainerBlockZ())
				{
					m_Collision = i;
					collision = true;
				}
			}
			else if (m_owner->getType() == PLAYER){
				if (DoBoxesIntersectSoft(m_owner, i)){
					m_Collision = i;
					collision = true;
				}
			}
			else if (DoBoxesIntersect(m_owner, i))
			{
				m_Collision = i;
				collision = true;
			}
		}
	}
	if (!collision){
		m_Collision = NULL;
	}
}

GameEntity *ColliderComponent::checkCollision(void){
	return m_Collision;
};


// ================ TIMER COMPONENT ============================== //

TimerComponent::TimerComponent(const TimerComponent & _gComp)
{
	static_cast<void>(_gComp);
	return;
}
TimerComponent & TimerComponent::operator=(const TimerComponent & _rhs)
{
	static_cast<void>(_rhs);
	return *this;
}

TimerComponent::TimerComponent(void){
	return;
}

void	TimerComponent::update(void){
	UpdateElapsed(m_owner->getWorld()->getDeltaTime());
	return;
}

TimerComponent::TimerComponent(GameEntity *owner)
: Component(owner)
{
	mType = E_COMPONENT_TYPE::COMPONENT_TIMER;
	Reset();
	return;
}

TimerComponent::~TimerComponent(void){
	return;
}

void TimerComponent::Reset(void) {
	// std::cout << "component TIMER: " << mElapsedTime << std::endl;
	mElapsedTime = 0.0f;
}

void TimerComponent::UpdateElapsed(float DeltaTime){
	//ticks in miliseconds (/1000 for microsecs)
	mElapsedTime += DeltaTime;
}

void 	TimerComponent::setElapsed(float _elapsed){
	mElapsedTime = _elapsed;
}

float 	TimerComponent::getElapsed(void){
	return mElapsedTime;
}

bool TimerComponent::checkTimer(float time){
	
	if (mElapsedTime >= time)
		return true;
	return false;
}


// ================ ANIMATION COMPONENT ============================== //

//cononical start
// AnimationComponent::AnimationComponent(void){
// 	return;
// };
// AnimationComponent::~AnimationComponent(void){
// 	return;
// };
// AnimationComponent::AnimationComponent(const AnimationComponent & animu){
// 	static_cast<void>(animu);
// 	return;
// };
// AnimationComponent & AnimationComponent::operator=(const AnimationComponent &rhs){
// 	static_cast<void>(rhs);
// 	return *this;
// };
//cononical start
AnimationComponent::AnimationComponent(const AnimationComponent & _gComp)
{
	static_cast<void>(_gComp);
	return;
}
AnimationComponent & AnimationComponent::operator=(const AnimationComponent & _rhs)
{
	static_cast<void>(_rhs);
	return *this;
}
AnimationComponent::AnimationComponent(void)
{

}

AnimationComponent::~AnimationComponent(void)
{

}

AnimationComponent::AnimationComponent(GameEntity *_owner)
:Component(_owner)
{
	mType = E_COMPONENT_TYPE::COMPONENT_ANIMATION;
	timer = new TimerComponent(_owner);
}

void AnimationComponent::setAnimationMode(E_ANIMATION_TYPE _mode){
	if (mAnimationState == ABILITY && _mode == IDLE){
        if (mCurrentAnimation == m_models_ability.size()-1){
            mAnimationState = _mode;
            mCurrentAnimation = 0;
            timer->Reset();
        }
    }
    else if (mAnimationState != _mode){
        mAnimationState = _mode;
        mCurrentAnimation = 0;
        timer->Reset();
    }
}

void AnimationComponent::setScaleAnimation(float scale, E_ANIMATION_TYPE onAnimation){
	mAnimationToScaleOn = onAnimation;
	mScaleMultiplier = scale;
};

void AnimationComponent::scaleAnimation(void){
	if (mAnimationToScaleOn == mAnimationState){
		auto graphics = dynamic_cast<GraphicsComponent*>(m_owner->getComponent(COMPONENT_GRAPHICS));
		graphics->multiplyScale(mScaleMultiplier);
	}
}

void AnimationComponent::create()
{

}

bool AnimationComponent::isCurrentAnimationComplete(void){
	switch(mAnimationState){
		case RUN:
			return mRunInterval == 0.0f ?  true : mCurrentAnimation == m_models_run.size() - 1;
			break;
		case IDLE:
			return mIdleInterval == 0.0f ?  true : mCurrentAnimation == m_models_idle.size() - 1;
			break;
		case DEATH:
			return mDeathInterval == 0.0f ? true : mCurrentAnimation == m_models_death.size() - 1;
			break;
		case ABILITY:
			return mAbilityInterval == 0.0f ?  true :  mCurrentAnimation == m_models_ability.size() - 1;
			break;
		case ANIMATION_NONE:
			return true;
			break;
	}
	return false;
}

void AnimationComponent::loadModel(GameBoi::graphics::Model * _model, E_ANIMATION_TYPE _type){
	switch(_type){
		case RUN:
			m_models_run.push_back(_model);
			break;
		case IDLE:
			m_models_idle.push_back(_model);
			break;
		case DEATH:
			m_models_death.push_back(_model);
			break;
		case ABILITY:
			m_models_ability.push_back(_model);
			break;
		case ANIMATION_NONE:
			return;
			break;
	}
};


void  AnimationComponent::update(){
	timer->update();

	E_ENTITY_TYPE type = m_owner->getType();
	Component *movementComponent;

	GraphicsComponent *graphics;
	
	if (mAnimationState != DEATH)
	{
		

		if (type == PLAYER){
			movementComponent = m_owner->getComponent(E_COMPONENT_TYPE::COMPONENT_MOVEMENT);
			setAnimationMode(dynamic_cast<PlayerMovementComponent*>(movementComponent)->getMoveState());
		}else if (type == ENEMY){
			movementComponent = m_owner->getComponent(E_COMPONENT_TYPE::COMPONENT_MOVEMENT);
			setAnimationMode(dynamic_cast<EnemyMovementComponent*>(movementComponent)->getMoveState());
		}
	}
	if (mAnimationState == RUN){
		// std::cout << "UPDATE RUN" << std::endl;

		if (mCurrentAnimation > m_models_run.size() - 1){
			mCurrentAnimation = 0;
		}

		if (timer->checkTimer(mRunInterval)){
			if (type == PLAYER)
				this->getOwner()->getWorld()->playSound(E_SOUNDS::E_SOUNDS_FOOTSTEPS, false);
			scaleAnimation();
			graphics = dynamic_cast<GraphicsComponent*>(m_owner->getComponent(COMPONENT_GRAPHICS));
			graphics->setModel(m_models_run[mCurrentAnimation]);
			mCurrentAnimation++;
			timer->Reset();
			
		}
	}
	else if (mAnimationState == IDLE){
		// std::cout << "UPDATE IDLE" << std::endl;

		if (mCurrentAnimation >= m_models_idle.size() - 1){
			mCurrentAnimation = 0;
		}
		if (timer->checkTimer(mIdleInterval)){
			scaleAnimation();
			graphics = dynamic_cast<GraphicsComponent*>(m_owner->getComponent(COMPONENT_GRAPHICS));
			graphics->setModel(m_models_idle[mCurrentAnimation]);
			mCurrentAnimation++;
			timer->Reset();
		}
	}
	else if (mAnimationState == ABILITY){
		// std::cout << "UPDATE ABILITY" << std::endl;

		if (mCurrentAnimation >= m_models_ability.size() - 1){
			mCurrentAnimation = 0;
		}
		if (timer->checkTimer(mAbilityInterval)){
			scaleAnimation();
			graphics = dynamic_cast<GraphicsComponent*>(m_owner->getComponent(COMPONENT_GRAPHICS));
			graphics->setModel(m_models_ability[mCurrentAnimation]);
			mCurrentAnimation++;
			timer->Reset();
		}
		if (isCurrentAnimationComplete()) {
			this->getOwner()->getWorld()->playSound(E_SOUNDS::E_SOUNDS_PLACE_BOMB, false);
			ColliderComponent* collider = dynamic_cast<ColliderComponent*>(m_owner->getComponent(COMPONENT_COLLIDER));
			collider->setContainingBlock();
			// std::cout << "collider->getContainerBlockX(): " << collider->getContainerBlockX() << std::end;
			m_owner->placeBomb(collider->getContainerBlockX(), collider->getContainerBlockZ());
		}
	}
	else if (mAnimationState == DEATH){
		// std::cout << "UPDATE DEATH" << std::endl;
		//death animation can only play once so dont reset
		if (mCurrentAnimation == 0 && type == PLAYER){
			m_owner->getWorld()->playSound(E_SOUNDS::E_SOUNDS_PLAYER_DEATH, false);
		}
		if (mCurrentAnimation == 0 && type == ENEMY){
			m_owner->getWorld()->playSound(E_SOUNDS::E_SOUNDS_ENEMY_DEATH, false);
		}
		if (mCurrentAnimation <= m_models_death.size() - 1){
			if (timer->checkTimer(mDeathInterval)){
				scaleAnimation();
				graphics = dynamic_cast<GraphicsComponent*>(m_owner->getComponent(COMPONENT_GRAPHICS));
				graphics->setModel(m_models_death[mCurrentAnimation]);
				mCurrentAnimation++;
				timer->Reset();
			}
		}
	}

	return;
}

E_ANIMATION_TYPE AnimationComponent::getCurrentAnimation(void){
	return mAnimationState;
}

void AnimationComponent::setInterval(float run, float idle, float death, float ability){
	mRunInterval = run;
	mIdleInterval = idle;
	mDeathInterval = death;
	mAbilityInterval = ability;
}


int AnimationComponent::getCurrentAnimationIndex(void){
	return mCurrentAnimation;
}

void AnimationComponent::setCurrentAnimationIndex(int _index){
	mCurrentAnimation = _index; 
}






//sick almost 1000 lines lol