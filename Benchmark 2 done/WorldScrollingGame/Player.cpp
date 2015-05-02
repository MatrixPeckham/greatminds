#pragma once
#include "Player.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "SparseLayer.h"
#include "SpriteManager.h"
#include "Projectile.h"
#include "GameMusic.h"

extern int GAME_MODE;

Player::Player(Game * g) {
	hp=100;
	ammo=25;
	bombs=10;
	weapon=1;
	game=g;
	canFire=true;
	hasMessage=false;
	framesToFire=FIRE_RATE;
}
wchar_t* Player::weaponToString() {
	switch(weapon) {
		case 0:
			return L"Current Weapon: Bombs";
		case 1:
			return L"Current Weapon: Bullets";
	}
	 return L"";
}
void Player::fire(Game *g, float angle, bool right,float unitXvel,float unitYvel){
	if(framesToFire>0) {

		return;
	}
	int bullet;
	SpriteManager* sm=g->getWorld()->getSpriteManager();
	int spriteIndex=findDeadWeapon(sm);
	Projectile* p=(Projectile*)sm->getSprite(spriteIndex);
	if(weapon==1) {
		if(ammo<=0) {
			((GameMusic*)g->getGameAudio())->playEffect("click");
			return;
		}
		ammo--;
		p->setSpriteType(sm->getSpriteType(sm->getBulletInd()));
		p->getBoundingVolume()->setHeight(2);
		p->getBoundingVolume()->setWidth(4);
		bullet=ammo;
	} else {
		if(bombs<=0) {
			((GameMusic*)g->getGameAudio())->playEffect("click");
			return;
		}
		bombs--;
		p->setSpriteType(sm->getSpriteType(sm->getBulletInd()-1));
		p->getBoundingVolume()->setHeight(32);
		p->getBoundingVolume()->setWidth(32);
		bullet=ammo;
	}
	p->right=right;
	p->angle=angle;
	//changed bv to pp getx and y:
	
	int wid = bv->getWidth()/2;
	int hig = bv->getHeight()/2;
	float xloc = bv->getX()+wid;
	float yloc = bv->getY()+hig;
	/*
	float tx = fabsf(wid/unitXvel);
	float ty = fabsf(hig/unitYvel);
	float t;
	if(unitYvel!=0){
		if((unitXvel/unitYvel)>(wid/hig)){
			t=tx;
		} else {
			t=ty;
		}
	} else {
		t=tx;
	}
	xloc+=t*unitXvel;
	yloc+=t*unitYvel;*/

	((GameMusic*)g->getGameAudio())->playEffect("fire");
	
	p->setXY(xloc,yloc);
	p->setWeapon(this->weapon,unitXvel,unitYvel);
	p->dead=false;
	p->life=LIFETIME;
	if(GAME_MODE==0) {
		framesToFire=FIRE_RATE;
	} else {
		framesToFire=FIRE_RATE;
	}
}
int Player::findDeadWeapon(SpriteManager* sm) {
		int i=sm->getWeaponsStartIndex();
		while(i<sm->getNumberOfSprites() && !((Projectile*)sm->getSprite(i))->dead) {
			i++;
		}
		return i;
}
void Player::switchWeapon(){
	++weapon;
	weapon%=NUM_WEAPONS;
}
void Player::updateSprite() {
	pp->incVelocityX(pp->getAccelerationX());
	pp->incVelocityY(pp->getAccelerationY());
	animationCounter++;
	animationCounter = animationCounter%spriteType->getAnimationSpeed();
	
	// WE ONLY CHANGE THE FRAME OF ANIMATION ONCE EVERY animationSpeed FRAMES
	if (animationCounter == 0)
	{
		changeFrame();
	}

	// UPDATE THE SPRITE LOCATION
	pp->incX();
	pp->incY();
	bv->incX(pp->getVelocityX());
	bv->incY(pp->getVelocityY());
	if(framesToFire>0) {
		framesToFire--;
	}
}