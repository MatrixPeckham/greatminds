#pragma once 
#include "KeyEventHandler.h"
#include "Game.h"
#include "AnimationStates.h"
#include "stdafx.h"

class KeyHandler : public KeyEventHandler {
public:
	KeyHandler();
	~KeyHandler();
	void setFacingRight(bool val) {facingRight=val;}
	bool getFacingRight() {return facingRight;}
	void handleKeyEvents(Game* g);
private:
	bool facingRight;
};