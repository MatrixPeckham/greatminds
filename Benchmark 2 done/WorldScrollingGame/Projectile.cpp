#pragma once
#include "Projectile.h"
#include "Constants.h"
#include "PhysicalProperties.h"
#include "BoundingVolume.h"
#include <math.h>
Projectile::Projectile() {
	dead=true;
	
	angle=0;
}
Projectile::~Projectile() {
}

void Projectile::setXY(float x, float y) {
	pp->setX(x);
	pp->setY(y);
	bv->setX(x);
	bv->setY(y);
//	bv->setWidth(2);
//	bv->setHeight(4);
}

void Projectile::setWeapon(int i,float unitXvel,float unitYvel) {
	type=i;
	switch(type) {
		case BOMB://bomb
			pp->setAccelerationY(GRAVITY);
			speed=BOMBSPEED;
			break;
		case BULLET:
			pp->setAccelerationY(0);
			speed=BULLETSPEED;
			break;
	}
	if(right) {
		pp->setVelocityX(speed*unitXvel);
		pp->setVelocityY(speed*unitYvel);
	} else {
		pp->setVelocityX(speed*unitXvel);
		pp->setVelocityY(speed*unitYvel);
	}

}
void Projectile::updateSprite() {
	
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
	bv->setX(pp->getX());
	bv->setY(pp->getY());
	life--;
	if(life<=0 || (pp->getVelocityX()==0 && pp->getVelocityY()==0)){
		dead=true;
	}
}