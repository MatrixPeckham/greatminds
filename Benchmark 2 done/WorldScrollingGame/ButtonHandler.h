#pragma once 
#include "ButtonEventHandler.h"
#include "Game.h"
#include "stdafx.h"
#include "MoreGameStates.h"

class ButtonHandler : public ButtonEventHandler {
private: 
	wchar_t* com;
	int lastState;
public:
	ButtonHandler();
	~ButtonHandler();
	void handleButtonEvents(Game *game, wchar_t *command);
};