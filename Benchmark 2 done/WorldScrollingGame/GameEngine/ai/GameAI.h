#pragma once

/*	
	Author: _______________

	GameAI.h

	This class will be used to manage all game bots, and the
	Artificial Intelligence associated with them. Each frame
	we will update the AI state, which should update all bots
	accordingly.

	Students will implement this as part of their projects.
*/

// PREPROCESSOR INCLUDE STATEMENTS
#include "resource.h"
#include "stdafx.h"
#include "Game.h"
#include <vector>
#include "Bot.h"
#include "DeterministicBot.h"

class GameAI
{
private:
	vector<Bot*>* bots;
	int airemaining;
public:
	// WE'LL DEFINE THESE METHODS LATER
	GameAI();
	~GameAI();
	int getAIRemaining(){return airemaining;}
	void update(Game *game);
	void init(Game * game);
	Bot* getBotAt(int i) {return bots->at(i);}
	void clear();
	void addBot(Bot* b) {bots->push_back(b);}
	int getNumBots(){return bots->size();}
};