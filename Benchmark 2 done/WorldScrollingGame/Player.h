#pragma once
#include "Game.h"
#include "AnimatedSprite.h"
#include "Constants.h"
#include "SpriteManager.h"

#define FIRE_RATE 25
class Player : public AnimatedSprite {
public:
	Player(Game *g);
	~Player(){}
	int hp;
	int ammo;
	int framesToFire;
	int bombs;
	int weapon;
	int team;
	bool canFire;
	bool hasMessage;
	Game *game;
	int findDeadWeapon(SpriteManager* sm);
	virtual void fire(Game* g, float angle, bool r,float unitXvel,float unitYvel);
	void switchWeapon();
	wchar_t* weaponToString();
	void updateSprite();
	void setTakeFlag(bool b) { takeFlag=b;}
	bool canTakeFlag()       {return takeFlag;}


private:
	bool takeFlag;
};
	
