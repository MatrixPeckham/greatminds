
#pragma once
#include "stdafx.h"
#include "TextGenerator.h"

class WRTextGenerator : public TextGenerator
{
public:
	WRTextGenerator()	{}
	~WRTextGenerator()	{}
	void initText(Game *game);
	void updateText(Game *game);
}
;