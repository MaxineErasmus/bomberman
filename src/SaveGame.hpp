#pragma once
#include <fstream>
#include <iostream>
#include "Component.hpp"

class SaveGame

{

public:

	float		Xpos;
	float		Ypos;
	float		Zpos;
	int			type;
	int			zDir;
	int			xDir;
	int			level;
	float		speed;
	float		range;
	float		elapsedTime;
	int 		lives;
	int			score;
	E_ANIMATION_TYPE currentAnimation;
	int animationIndex;
	int isAlive;
};