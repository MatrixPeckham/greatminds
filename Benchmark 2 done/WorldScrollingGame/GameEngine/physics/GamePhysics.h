/*	
	Author: _______________

	GamePhysics.h

	This class will be used to manage all game collisions
	and physics. This includes collision detection, and resolution.
	Each frame we will update the collided objects velocities and
	positions accordingly.

	Students will implement this as part of their projects.
*/

#pragma once
#include "resource.h"
#include "stdafx.h"
#include "Game.h"

class GamePhysics
{
public:
	// WE'LL DEFINE THIS CLASS LATER
	GamePhysics() {}
	~GamePhysics(){}
	virtual void update(Game *game) {}
};