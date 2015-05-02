#pragma once
#include "CollidableObject.h"
#include "AnimatedSprite.h"
class collision {
public:
	
	AnimatedSprite* o1;
	AnimatedSprite* o2;
	float firstTime;
	bool x;
	int right;
	int top;
	float firstXTime;
	float firstYTime;
	float lastXTime;
	float lastYTime;
	bool operator<(const collision &obj2) const {
		return firstTime<obj2.firstTime;
	}
	
};