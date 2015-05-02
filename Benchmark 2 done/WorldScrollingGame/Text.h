#pragma once
#include "TextGenerator.h"
#include "Game.h"

class Text : public TextGenerator{
public:
	Text() {}
	~Text(){}
	void initText(Game *game);
	void updateText(Game *game);
};