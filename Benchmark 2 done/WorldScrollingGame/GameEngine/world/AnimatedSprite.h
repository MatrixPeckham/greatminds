/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSprite.h

	This class represents a sprite that can can
	be used to animate a game character or object.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include "AnimatedSpriteType.h"
#include "BoundingVolume.h"
#include "CollidableObject.h"
#include "PhysicalProperties.h"
#include "Viewport.h"
#include <vector>

// WE COULD KEEP TRACK OF ANIMATION STATES
// USING CONSTANTS IF WE LIKE
const int STANDING_FACING_RIGHT = 0;
const int STANDING_FACING_LEFT = 1;
const int STANDING_FACING_UP = 2;
const int STANDING_FACING_DOWN = 3;
const int RUNNING_RIGHT = 4;
const int RUNNING_LEFT = 5;
const int RUNNING_UP = 6;
const int RUNNING_DOWN = 7;

class AnimatedSprite : public CollidableObject
{
protected:
	// SPRITE TYPE FOR THIS SPRITE. THE SPRITE TYPE IS THE ID
	// OF AN AnimatedSpriteType OBJECT AS STORED IN THE SpriteManager
	AnimatedSpriteType *spriteType;

	// TRANSPARENCY/OPACITY
	int alpha;

	// ANIMATION SEQUENCE CURRENTLY IN USE
	int currentState;

	// FRAME OF ANIMATION CURRENTLY BEING USED FOR currentState
	int currentFrame;

	// THE INDEX OF THE CURRENT FRAME IN THE ANIMATION SEQUENCE
	int frameIndex;

	// USED TO ITERATE THROUGH THE CURRENT ANIMATION SEQUENCE
	int animationCounter;

public:
	// INLINED ACCESSOR METHODS
	int					getAlpha()			{ return alpha;			}
	int					getCurrentFrame()	{ return currentFrame;	}
	int					getCurrentState()	{ return currentState;	}
	int					getFrameIndex()		{ return frameIndex;	}
	AnimatedSpriteType*	getSpriteType()		{ return spriteType; }

	// INLINED MUTATOR METHODS
	void setAlpha(int initAlpha)
	{	alpha = initAlpha;				}
	void setSpriteType(AnimatedSpriteType *initSpriteType)
	{	spriteType = initSpriteType;	}

	// METHODS DEFINED IN AnimatedSprite.cpp
	AnimatedSprite();
	~AnimatedSprite();
	void changeFrame();
	void setCurrentState(int newState);
	virtual void updateSprite();
};