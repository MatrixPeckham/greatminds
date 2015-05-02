#pragma once
#include "AnimatedSprite.h"

class Explosion : public AnimatedSprite{
public:
	Explosion();
	~Explosion();
	bool dead;
	void updateSprite();
	void setXY(float x, float y);
	void setDead(bool d){dead=d;}
};