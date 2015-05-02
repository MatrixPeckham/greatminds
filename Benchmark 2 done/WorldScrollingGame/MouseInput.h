#pragma once 
#include "Game.h"
#include "WindowsGameInput.h"
#include "GameInput.h"
class MouseInput : public WindowsGameInput {
public:
	MouseInput(){}
	~MouseInput(){}
	void gameInput(int i, Game* g);
};