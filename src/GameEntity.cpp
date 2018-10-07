#include "GameEntity.hpp"
#include "Component.hpp"
#include "GameWorld.hpp"
#include "Builder.hpp"

std::vector<Component*> *GameEntity::getComponents()
{
	return &mComponentList;
}

Component *GameEntity::getComponent(E_COMPONENT_TYPE type)
{
	for (auto i : mComponentList){
		if (i->getType() == type){
			return i;
		}
	}
	return NULL;
}

void GameEntity::addComponent(Component *component)
{
	mComponentList.push_back(component);
}

GameEntity::GameEntity(GameWorld *world, bool canDestroy)
{
	m_world = world;
	mCanDestroy = canDestroy;
	mIsAlive = true;
	mPosition = glm::vec3(1.0f, 1.0f, 1.0f);
	mType = E_ENTITY_TYPE::NONE;
	mBombRange = 0.0f;
}


GameEntity::~GameEntity(void){
	//kill component list
	for (auto i : mComponentList){
		// std::cout << "DELETING COMPONENT: " << i->getType() << std::endl;
		delete i;
		// std::cout << "DONE "<< std::endl;

	}
}


GameEntity::GameEntity(const GameEntity &src){
	static_cast<void>(src);
	return;
}

GameEntity &GameEntity::operator=(const GameEntity &rhs){
	static_cast<void>(rhs);
	return *this;
}


void GameEntity::setId(int id){
	mId = id;
}

int GameEntity::getId(void) const{
	return mId;
}

void GameEntity::setType(E_ENTITY_TYPE type)
{
	mType = type;
}

E_ENTITY_TYPE GameEntity::getType()
{
	return mType;
}

GameWorld *GameEntity::getWorld()
{
	return m_world;
}

glm::vec3 *GameEntity::getPosition()
{
	return &mPosition;
}

void GameEntity::setPosition(float x, float y, float z)
{
	mPosition = glm::vec3(x,y,z);
}

void GameEntity::update()
{
	//ONLY RESPONSIBLE FOR MOVING ENTITY && TIMER CHECKS NOT RENDERING
	if (isAlive()){
		for (auto i : mComponentList) {
			// std::cout << "UPDATE COMPONENT: " << i->getType() << std::endl;
	  		
	  		i->update();
			// std::cout << "UPDATED: " << i->getType() << std::endl;

		}
	}
		// std::cout << "UPDATE COMPONENT COMPLETED: " << getType() << std::endl;

	//if entity is a bomb check the timer
	if (getType() == E_ENTITY_TYPE::BOMB){
		TimerComponent * timer = dynamic_cast<TimerComponent*>(getComponent(E_COMPONENT_TYPE::COMPONENT_TIMER));

		if (timer->checkTimer(3.0f) && mIsAlive)
		{
			detonateBomb();
			kill();
		}
	}
	if (getType() == E_ENTITY_TYPE::EXPLOSION){
		TimerComponent * timer = dynamic_cast<TimerComponent*>(getComponent(E_COMPONENT_TYPE::COMPONENT_TIMER));
		if (timer->checkTimer(0.15))
		{
			kill();
		}
	}

}

void GameEntity::setPosition(glm::vec3 position)
{
	mPosition = position;
}

void GameEntity::dropBombCount(void){
	numBombs--;
	if (numBombs < 0){
		numBombs = 0;
	}
}

void GameEntity::placeBomb(float x, float z){
	
	if (numBombs >= static_cast<int>(mBombRange)){
		//play a no no sound
		m_world->playSound(E_SOUNDS::E_SOUNDS_NoNo, false);
		return;
	}

	numBombs++;
	//only players can place a bomb	
	if (getType() != E_ENTITY_TYPE::PLAYER){
		std::cout << "ENTITY OF TYPE: "<< getType() << " cannot place bomb" << std::endl;
		return;
	}
	Builder * builder = getWorld()->getBuilder();
	std::cout << "placing bomb at x: " << x << " z: " << z << std::endl;
	builder->createBomb(x, getPosition()->y, z);
	return;
}


void GameEntity::detonateBomb(){
	if (getType() != E_ENTITY_TYPE::BOMB){
		std::cout << "ENTITY OF TYPE: "<< getType() << " cannot detonate" << std::endl;
		return;
	}
	int left = 0;
	int right = 0;
	int up = 0;
	int down = 0;

	float bombRaduis = getBombRange();
	float scale = m_world->getScaling();
	float originalX = getPosition()->x;
	float originalY = getPosition()->y;
	float originalZ = getPosition()->z;
	Builder * builder = static_cast<Builder*>(m_world->getBuilder());
	m_world->playSound(E_SOUNDS::E_SOUNDS_EXPLOSION, false);
	for (float raduis = 0.0f; raduis <= bombRaduis; raduis+=scale){
		for (auto i : *(m_world->getEntityList())){
			if(i->getId() == mId || i->getType() == INVISIBLEWALL){
				continue;
			}
			auto entityCollider = dynamic_cast<ColliderComponent*> (i->getComponent(E_COMPONENT_TYPE::COMPONENT_COLLIDER));
			if (entityCollider){
				if (entityCollider->isWithinBlock(originalX, originalZ)){	
					entityCollider->setContainingBlock();
					if (originalX == entityCollider->getContainerBlockX() && originalZ == entityCollider->getContainerBlockZ()){
						if (i->isDestroyable()){
							i->kill();
						}
					}
				}
				if (left!=2){
					if (entityCollider->isWithinBlock(originalX + raduis ,originalZ)){
						entityCollider->setContainingBlock();
						if (originalX + raduis == entityCollider->getContainerBlockX() && originalZ == entityCollider->getContainerBlockZ()){
							if (i->isDestroyable()){
								i->kill();
							}
							if (raduis != 0)
								left = 1;
						}
					}
				}
				if (right!=2){
					if (entityCollider->isWithinBlock(originalX - raduis ,originalZ)){
						entityCollider->setContainingBlock();
						if (originalX - raduis == entityCollider->getContainerBlockX() && originalZ == entityCollider->getContainerBlockZ()){
							if (i->isDestroyable()){
								i->kill();
							}
							if (raduis != 0)
								right = 1;
						}
					}
				}
				if (up!=2){
					if (entityCollider->isWithinBlock(originalX ,originalZ + raduis)){
						entityCollider->setContainingBlock();
						if (originalX  == entityCollider->getContainerBlockX() && originalZ + raduis == entityCollider->getContainerBlockZ()){
							if (i->isDestroyable()){
								i->kill();
							}
							if (raduis != 0)
								up = 1;
						}
					}
				}
				if (down!=2){
					if (entityCollider->isWithinBlock(originalX ,originalZ - raduis)){
						entityCollider->setContainingBlock();
						if (originalX  == entityCollider->getContainerBlockX() && originalZ - raduis == entityCollider->getContainerBlockZ()){
							if (i->isDestroyable()){
								i->kill();
							}
							if (raduis != 0)
								down = 1;
						}
					}
				}
			}
		}
		if (left == 0 || left == 1){
			builder->createExplosion(originalX + raduis, originalY, originalZ);
		}
		if (right == 0 || right == 1){
			builder->createExplosion(originalX - raduis, originalY, originalZ);
		}
		if (up == 0 || up == 1){
			builder->createExplosion(originalX , originalY, originalZ + raduis);
		}
		if (down == 0 || down == 1){
			builder->createExplosion(originalX, originalY, originalZ - raduis);
		}
		if (left!=0)
			left = left == 1 || left == 2 ? 2 : 1;
		if (right!=0)
			right = right == 1 || right == 2 ? 2 : 1;
		if (up!=0)
			up = up == 1 || up == 2 ? 2 : 1;
		if (down!=0)
			down = down == 1 || down == 2 ? 2 : 1;
	}
}

void GameEntity::kill(void){

	mIsAlive = false;
}

int GameEntity::getLives(void){
	return mLives;
}

void GameEntity::setLives(int _lives){
	mIsAlive = true;
	this->mLives = _lives;
}

void GameEntity::setBombRange(float range){
	mBombRange = range;
}

float GameEntity::getBombRange(void){
	return mBombRange;
}

bool GameEntity::isAlive(void){

	return mIsAlive;
}

bool GameEntity::isDestroyable(void){
	return mCanDestroy;
}

void GameEntity::randomDrop(void){
	Builder *bobDatBuilder = m_world->getBuilder();

	int randomIndex = static_cast<int>(rand() % 10 + 1);
	if (randomIndex == 3){
		bobDatBuilder->createPowerUp(getPosition()->x,getPosition()->y, getPosition()->z, 1);
	} else if (randomIndex == 5){
		bobDatBuilder->createPowerUp(getPosition()->x,getPosition()->y, getPosition()->z, 2);
	}else if (randomIndex == 7){
		bobDatBuilder->createPowerUp(getPosition()->x,getPosition()->y, getPosition()->z, 3);
	}

}

void GameEntity::DropEndGame(void){
	Builder *bobDatBuilder = m_world->getBuilder();
	bobDatBuilder->createPortal(getPosition()->x,getPosition()->y, getPosition()->z);
}


void GameEntity::destroy(void){
	mIsAlive = false;
}