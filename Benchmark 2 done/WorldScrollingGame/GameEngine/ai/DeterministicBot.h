
#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "resource.h"
#include "stdafx.h"
#include "Game.h"
#include "Bot.h"
#include "Player.h"
#include "Constants.h"
#define GUARD		0
#define PURSUE		1
#define ATTACK		2
class DeterministicBot : public Bot
{
private: 
	int strategy;
	int team;
public:
	void setPlayer(int s, AIPlayer* p, int t) {
			animatedSprite=p;
			strategy=s;
			team=t;
	}
	void setStrategy(int s) {strategy=s;}
	int getStrategy(){return strategy;}
	void update(Game* g);
	void pursue(Game* g);
	void attack(Game* g);
	void guard(Game* g);
	void retreat(Game* g);

};