#pragma once
#include "ResourceManager.hpp"
#include "GameEntity.hpp"

class GameEntity;
class GameWorld;

class Builder 
{
private:
    GameWorld *m_world;
    ResourceManager *m_resources;
    float mGameScale;
    std::string mLevel;
public:
    Builder(void);
    ~Builder(void);
    Builder(const Builder &src);
    Builder &operator=(const Builder &rhs);

    Builder(GameWorld *world);

    void setGameScale(float scale);
    void create(GameWorld *world, ResourceManager *resources);

    GameEntity *createStartingGate(double x, double y, double z);
    GameEntity *createPlayer(double x, double y, double z);
    GameEntity *createEnemy(double x, double y, double z);
    void createWall(double x, double y, double z, bool canDestroy);
	void createEndWall(double x, double y, double z, bool canDestroy);
	void createPortal(double x, double y, double z);
    GameEntity * createBomb(double x, double y, double z);
    void createFloor(double x, double y, double z);
    void createPowerUp(double x, double y, double z, int type);
    void createExplosion(double x, double y, double z);
    void setLevel(std::string level);
};

