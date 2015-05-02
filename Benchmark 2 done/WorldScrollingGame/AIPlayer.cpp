#include "StdAfx.h"
#include "AIPlayer.h"
#include "Player.h"
#include "GameTimer.h"
#include <math.h>
AIPlayer::AIPlayer(Game * g) : Player(g)
{
	srand(g->getTimer()->getCurrentTime());
	lowFireRate = 1;
	highFireRate = 2;
	hp = 25;
	accuracy = 5;
	ammo = 100000;
	bombs = 1000;
	hitFrames=0;
	hit=false;
}

AIPlayer::~AIPlayer(void)
{
}
void AIPlayer::updateSprite() {
	Player::updateSprite();
	if(hit && hitFrames==0) {
		hitFrames=7;
		
	} 
	if(hit) {
		if(this->getCurrentState()==0 || this->getCurrentState()==3) {
			this->setCurrentState(3);
			this->changeFrame();
		} else {
			this->setCurrentState(4);
			this->changeFrame();
		}
	}
	if(hitFrames>0) {
		this->framesToFire=30;
		hitFrames--;
	} 
	if(hitFrames==0) {
		hit=false;
	}
	
}
void AIPlayer::fire(Game * g, float angle, bool right,float unitXvel,float unitYvel){

	if(framesToFire!=0) return;
	int a = 0;
	if(accuracy!=0)
		a = rand() % (2 * accuracy) - accuracy;
	const float PI = 3.1415926535;
	float diffAngle = (PI/180)*a;
	angle+=diffAngle;
	unitXvel+=sin(diffAngle);
	unitYvel+=sin(diffAngle);
	float mag = sqrt((unitXvel*unitXvel)+(unitYvel*unitYvel));
	unitXvel/=mag;
	unitYvel/=mag;
	bool wep = rand()%5==0;
	if(wep){switchWeapon();}
	Player::fire(g,angle,right,unitXvel,unitYvel);
	if(wep){switchWeapon();}
	if(framesToFire == FIRE_RATE)
		framesToFire = rand()%(highFireRate-lowFireRate)+lowFireRate;
}