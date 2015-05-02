#pragma once
#include "GameDataLoader.h"

class Game;

class LevelLoader : public GameDataLoader {
public:
	LevelLoader(){};
	~LevelLoader(){};
	void loadGUI(Game *game);
	void loadWorld(Game *game);

}