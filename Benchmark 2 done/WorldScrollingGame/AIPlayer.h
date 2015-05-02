#pragma once
#include "player.h"

class AIPlayer : public Player
{
private:
	int lowFireRate;
	int highFireRate;
	int accuracy;
	bool hit;
	int hitFrames;
public:
	AIPlayer(Game * g);
	~AIPlayer(void);
	void setHit(bool b) {hit=b;}
	void fire(Game * g, float andle, bool right,float unitXvel,float unitYvel);
	void setLowFireRate(int i){lowFireRate=i;}
	void setHighFireRate(int i){highFireRate=i;}
	void setAccuracy(int i){accuracy=i;}
	void updateSprite();
};
