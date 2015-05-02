#pragma once
#include "PhysicalProperties.h"
#include "AnimatedSprite.h"
#include "Explosion.h"
#include "BoundingVolume.h"

Explosion::Explosion() {
	dead=true;
}
Explosion::~Explosion() {
}

void Explosion::setXY(float x, float y) {
	pp->setX(x);
	pp->setY(y);
	bv->setX(x);
	bv->setY(y);
	bv->setWidth(32);
	bv->setHeight(32);
}

void Explosion::updateSprite() 
{	
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
	if(this->getFrameIndex() == 7)
	{
		this->setCurrentState(1);
	}
}