#pragma once
#include "AnimatedSprite.h"
#include "CollidableObject.h"
#define BOMB	0
#define BULLET	1
#define BULLETSPEED 15
#define BOMBSPEED	15
#define LIFETIME	70
class Projectile : public AnimatedSprite{
public:
	Projectile();
	~Projectile();
	int textureId;
	int type;
	int life;
	bool dead;
	bool right;
	float speed;
	float angle;
	void updateSprite();
	void setWeapon(int i,float unitXvel,float unitYvel);
	void setXY(float x, float y);
	void setAngle(float a) {angle=a;}
	void setDead(bool d){dead=d;}
};