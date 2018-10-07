#pragma once
#include <glm/glm.hpp>
#include "../GameEngine/graphics/includes/Functions.hpp"
#include "../GameEngine/graphics/includes/Shader.hpp"
#include "../GameEngine/utils/includes/timer.hpp"
// #include "../GameEngine/graphics/src/Textures.hpp"
#include "../GameEngine/graphics/includes/Camera.hpp"
#include "../GameEngine/graphics/includes/Models.hpp"

class GameEntity;

enum E_COMPONENT_TYPE 
{
	COMPONENT_DEFAULT,
	COMPONENT_GRAPHICS,
	COMPONENT_MOVEMENT,
	COMPONENT_COLLIDER,
	COMPONENT_ANIMATION,
	COMPONENT_TIMER,
};


enum E_DIRECTIONS 
{
	NORTH,
	EAST,
	SOUTH,
	WEST,
};

enum E_ANIMATION_TYPE 
{
	IDLE,
	RUN,
	DEATH,
	ABILITY,
	ANIMATION_NONE,
};

class Component 
{
protected:
	E_COMPONENT_TYPE mType;
	GameEntity *m_owner;

	Component();
    Component(const Component &src);
    Component &operator=(const Component &rhs);
public:
	Component(GameEntity *owner);
	GameEntity *getOwner();

	E_COMPONENT_TYPE getType();

	virtual void update() = 0;
	virtual ~Component(void);
};

class GraphicsComponent : public Component 
{
private: 
    GameBoi::graphics::Shader *m_shader;
    GameBoi::graphics::Model *m_model;

    glm::vec3 mOriginalScale;
	glm::vec3 mScale;
	glm::vec3 mSize;
	float mRotation;

	GraphicsComponent();
    GraphicsComponent(const GraphicsComponent &src);
    GraphicsComponent &operator=(const GraphicsComponent &rhs);
public:
;

	GraphicsComponent(GameEntity *owner);


	void create(GameBoi::graphics::Shader *shader, GameBoi::graphics::Model *model, glm::vec3 scale, float rotation, glm::vec3 size);
	void render();
	void update();

	glm::vec3 &getSize();
	void setSize(glm::vec3 size);
	void multiplyScale(float scale);

	void setRotation(float rot);

	GameBoi::graphics::Model *getModel();
	void setModel(GameBoi::graphics::Model *_model);
	~GraphicsComponent(void);

};

class PlayerMovementComponent : public Component 
{
private: 
	GraphicsComponent *m_graphics;
	float mSpeed;
	E_ANIMATION_TYPE mState;

	PlayerMovementComponent(const PlayerMovementComponent & _gComp);
	PlayerMovementComponent & operator=(const PlayerMovementComponent & _rhs);
	PlayerMovementComponent(void);
public:
	~PlayerMovementComponent();
	PlayerMovementComponent(GameEntity *owner);
	void create(GraphicsComponent *graphics, float speed);
	void update();
	void handleCollision(float xDir, float zDir, float distance);
	bool checkBombPlacement(float x, float z, GameEntity * b);
	float getSpeed();
	void setSpeed(float speed);
	E_ANIMATION_TYPE getMoveState(void);
};

class EnemyMovementComponent : public Component 
{
private:
	GraphicsComponent *m_graphics;
	std::vector<int> mPossibleMoves;
	float mSpeed;
	int xDirection;
	int zDirection;
	E_ANIMATION_TYPE mState;

	EnemyMovementComponent(const EnemyMovementComponent & _gComp);
	EnemyMovementComponent & operator=(const EnemyMovementComponent & _rhs);
	EnemyMovementComponent(void);
public:


	EnemyMovementComponent(GameEntity *owner);
	void	create(GraphicsComponent *graphics, float speed);
	void	update();
	void 	handleCollision(float xDir, float zDir, float distance);
	void	possibleMoves(void);
	void 	randomDirection(void);
	float	getSpeed();
	void	setSpeed(float speed);
	int 	getXDir(void);
	int 	getZDir(void);
	void	setDirection(int x, int z);
	E_ANIMATION_TYPE getMoveState(void);
	~EnemyMovementComponent();
};


class ColliderComponent : public Component 
{
private:
	GameEntity *m_Collision;
	float mXSize;
	float mYSize;
	float mContainingBlockX;
	float mContainingBlockZ;

	ColliderComponent(const ColliderComponent & _gComp);
	ColliderComponent & operator=(const ColliderComponent & _rhs);
	ColliderComponent(void);

public:
	~ColliderComponent();

	ColliderComponent(GameEntity *owner);
	void create(float xSize, float ySize);
	void update();
	void setCollision(void);
	void clearCollision(void);
	GameEntity *checkCollision(void);
	bool checkIntersection(GameEntity * _a, GameEntity * _b);
	bool isWithinBlock(float blockX, float blockZ);
	void setContainingBlock(void);
	float getContainerBlockX(void);
	float getContainerBlockZ(void);
	float getXSize();
	float getYSize();
};

class TimerComponent : public Component 
{
private:
	float	mElapsedTime;

	TimerComponent(const TimerComponent & _gComp);
	TimerComponent & operator=(const TimerComponent & _rhs);

public:

	TimerComponent(void);
	~TimerComponent(void);

	TimerComponent(GameEntity *owner);

	void	Reset(void);
	bool	checkTimer(float time);
	void 	setElapsed(float _elapsed);
	float	getElapsed(void);

	//update the clock with latest tick from game loop
	void	UpdateElapsed(float DeltaTime);
	void	update(void);
};


class AnimationComponent : public Component{
private:
	//vector pointing to respective models in resource manager
	//need to load this vector on construction 
	std::vector <GameBoi::graphics::Model *> m_models_run;
	std::vector <GameBoi::graphics::Model *> m_models_idle;
	std::vector <GameBoi::graphics::Model *> m_models_death;
	std::vector <GameBoi::graphics::Model *> m_models_ability;


	//animation mode so we know what state the player is in
	E_ANIMATION_TYPE mAnimationState;
	//interval will be the time between each animation switch also set on construction
	float mRunInterval;
	float mIdleInterval;
	float mDeathInterval;
	float mAbilityInterval;

	float   mScaleMultiplier;
	E_ANIMATION_TYPE mAnimationToScaleOn;

	//timer to keep track independantly
	TimerComponent *timer;
	size_t mCurrentAnimation;

	AnimationComponent(const AnimationComponent & _gComp);
	AnimationComponent & operator=(const AnimationComponent & _rhs);
	AnimationComponent(void);

public:
	//canonical end

	AnimationComponent(GameEntity *owner);
	void loadModel(GameBoi::graphics::Model * _model, E_ANIMATION_TYPE _type);
	void setScaleAnimation(float scale, E_ANIMATION_TYPE onAnimation);
	void scaleAnimation(void);
	void setInterval(float run, float idle ,float death, float ability);
	void setAnimationMode(E_ANIMATION_TYPE _mode);
	E_ANIMATION_TYPE getCurrentAnimation(void);
	bool isCurrentAnimationComplete(void);
	//need to update timer  and check here
	void create();
	void update(void);

	//savegame shit
	int getCurrentAnimationIndex(void);
	void setCurrentAnimationIndex(int _index);
	~AnimationComponent();

};
