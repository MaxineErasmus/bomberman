#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Component.hpp"

class GameWorld;

enum E_ENTITY_TYPE{
	NONE, 
	PLAYER,
	ENEMY,
	WALL,
	CRATE,
	BOMB,
	INVISIBLEWALL,
	SPEED_UP,
	LIFE_UP,
	BOMB_UP,
	EXPLOSION,
	ENDGAMEWALL,
	PORTAL,
	FLOOR,
};

class GameEntity
{
private:
	int mId;
	int mLives;
	bool mIsAlive;
	bool mCanDestroy;
	float mBombRange;
	int numBombs = 0;

	std::vector<Component*> mComponentList; 
	GameWorld *m_world;

	glm::vec3 mPosition;
	E_ENTITY_TYPE mType;
	

public: 
	GameEntity(GameWorld *world, bool canDestroy);
	~GameEntity(void);
	GameEntity(const GameEntity &src);
	GameEntity &operator=(const GameEntity &rhs);

	std::vector<Component*> *getComponents();
	void addComponent(Component *component);
	Component *getComponent(E_COMPONENT_TYPE type);

	void setId(int id);
	int  getId(void) const;

	glm::vec3 *getPosition();
	void setPosition(glm::vec3 position);
	void setPosition(float x, float y, float z);
	void placeBomb(float x, float z);
	void detonateBomb(void);
	void kill(void);
	void destroy(void);
	bool isAlive(void);
	bool isDestroyable(void);
	int  getLives(void);
	void setLives(int _lives);
	void randomDrop(void);
	void DropEndGame(void);
	float getBombRange(void);
	void  setBombRange(float _range);
	void dropBombCount(void);
	// void releaseComponents(void);

	void setType(E_ENTITY_TYPE type);
	E_ENTITY_TYPE getType();

	GameWorld *getWorld();
	void update();

};