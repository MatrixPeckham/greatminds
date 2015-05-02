#pragma once
#include "DeterministicBot.h"
#include "Player.h"
#include "AnimatedSprite.h"
#include "SpriteManager.h"
#include "Game.h"
#include "AnimationStates.h"
#include "GameWorld.h"
#include <math.h>
#define	 RANGE		50
void DeterministicBot::update(Game* g) {
	Player* p=((Player*)animatedSprite);
	p->ammo=100;
	p->bombs=20;
	if(p->hp>0) {
		switch(strategy) {
			case 0:
					 guard(g);
					break;
			case 1: pursue(g);
				break;
			case 2: attack(g);
				break;
			case 3:retreat(g);
				break;
		}
		if(p->hasMessage) {
			strategy=PURSUE;
		}
	} else {
		p->setCurrentState(DEAD);
	}
}
void DeterministicBot::pursue(Game* g){
	AnimatedSprite* p= g->getWorld()->getSpriteManager()->getPlayer();
	float dx=animatedSprite->getBoundingVolume()->getX()-p->getBoundingVolume()->getX();
	float dy=animatedSprite->getBoundingVolume()->getY()-p->getBoundingVolume()->getY();
	if(abs(p->getPhysicalProperties()->getX()-animatedSprite->getPhysicalProperties()->getX())<RANGE) {
		p->getPhysicalProperties()->setVelocityX(0);
	}
	if(sqrt(pow((double)dx,2)+pow((double)dy,2))<=RANGE) {
//		((Player*)animatedSprite)->fire(g,atan(dy/dx),false);
	}
}
void DeterministicBot::attack(Game* g){
}
void DeterministicBot::guard(Game* g){
	AnimatedSprite* p= g->getWorld()->getSpriteManager()->getPlayer();
	float dx=animatedSprite->getBoundingVolume()->getX()-p->getBoundingVolume()->getX();
	float dy=animatedSprite->getBoundingVolume()->getY()-p->getBoundingVolume()->getY();
	if(sqrt(pow((double)dx,2)+pow((double)dy,2))<=RANGE) {
//		((Player*)animatedSprite)->fire(g,atan(dy/dx),false);
	}
	
}
//float calculateXDistanceBetween(CollidableObject *o1, CollidableObject *o2) {

void DeterministicBot::retreat(Game* g){
}