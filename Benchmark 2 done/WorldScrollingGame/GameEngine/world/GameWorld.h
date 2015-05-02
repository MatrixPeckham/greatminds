/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameWorld.h

	This class manages the static data for the game level being
	used. This means all game backgrounds, which are stored and
	manipulated in WorldLayer objects. A given level can have
	a game background rendered using multiple layers, some which
	can be tiled, others that can be sparse, and others that
	can be isometric.

	This class stores these layers and ensures they are rendered
	in the proper order, low index to high. Layers that need to
	be drawn first (the back-most layer), should be added first.
*/
#pragma once
#include "resource.h"
#include "stdafx.h"
#include "GameDataLoader.h"
#include "RenderList.h"
#include "Viewport.h"
#include "WorldLayer.h"
#include <vector>

class Game;
class SpriteManager;

class GameWorld
{
private:
	// NOTE, THE worldWidth & worldHeight REPRESENT THE FULL
	// SIZE OF THIS LEVEL. WHICH WOULD BE AT LEAST THE SIZE OF
	// THE VIEWPORT, AND LIKELY MUCH LARGER, WHICH WOULD THEN
	// REQUIRE SCROLLING TO SEE IT.
	int worldWidth;
	int worldHeight;

	vector<WorldLayer*> *layers;

	// FOR MANAGING ANIMATED GAME OBJECTS FOR CURRENT LEVEL
	SpriteManager *spriteManager;

	// OUR WINDOW ON THE GAME WORLD FOR CURRENT LEVEL
	Viewport *viewport;

	// GAME BACKGROUND, STATIC OBJECTS, AND PARTICLE SYSTEMS FOR CURRENT LEVEL
	int currentLevel;

	// PATHS & NAMES OF DATA INPUT FILES FOR ALL GAME LEVELS
	vector<wchar_t*> *levelFileNames;
	vector<wchar_t*>::iterator levelFileNamesIterator;

public:
	// INLINED ACCESSOR METHODS
	vector<WorldLayer*>*	getLayers()	{ return layers;				}
	int						getWorldHeight()	{ return worldHeight;			}
	int						getWorldWidth()		{ return worldWidth;			}
	int						getCurrentLevel()	{ return currentLevel;			}
	int						getNumLevels()		{ return levelFileNames->size();}
	SpriteManager*			getSpriteManager()	{ return spriteManager;			}
	Viewport*				getViewport()		{ return viewport;				}

	// INLINED MUTATOR METHODS
	void setWorldHeight(int initWorldHeight)
	{ worldHeight = initWorldHeight;		}
	void setWorldWidth(int initWorldWidth)
	{ worldWidth = initWorldWidth;			}
	void setCurrentLevel(int l){currentLevel=l;}
	// METHODS DEFINED in GameWorld.cpp
	GameWorld();
	~GameWorld();
	void	addLayer(WorldLayer *layerToAdd);
	void	addLevelFileName(wchar_t *levelFileName);
	void	addWorldRenderItemsToRenderList(Game *game);
	wchar_t* getLevelFileName(int levelNumber);
	void	loadCurrentLevel(Game *game, int initLevel);
	void	unloadCurrentLevel(Game *game);
	void	update(Game *game);
};