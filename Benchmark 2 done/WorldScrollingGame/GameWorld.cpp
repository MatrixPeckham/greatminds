/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameWorld.cpp

	See GameWorld.h for a class description.
*/

#include "stdafx.h"
#include "Game.h"
#include "GameDataLoader.h"
#include "GameGraphics.h"
#include "GameWorld.h"
#include "RenderList.h"
#include "SpriteManager.h"
#include "TextureManager.h"
#include "World.h"
#include "GameAI.h"
#include "Viewport.h"
#include <vector>

/*
	GameWorld - Default Constructor, it constructs the layers
	vector, allowing new layers to be added.
*/
GameWorld::GameWorld()	
{
	layers = new vector<WorldLayer*>();
	worldWidth = 0;
	worldHeight = 0;
	currentLevel = 0;
	spriteManager = new SpriteManager();
	viewport = new Viewport();
	levelFileNames = new vector<wchar_t*>();
}

/*
	~GameWorld - This destructor will remove the memory allocated
	for the layer vector.
*/
GameWorld::~GameWorld()	
{
	delete layers;
	delete spriteManager;
	delete viewport;
	delete levelFileNames;
}

/*
	addLayer - This method is how layers are added to the World.
	These layers might be TiledLayers, SparseLayers, or 
	IsometricLayers, all of which are child classes of WorldLayer.
*/
void GameWorld::addLayer(WorldLayer *layerToAdd)
{
	layers->push_back(layerToAdd);
}

/*
	addWorldRenderItemsToRenderList - This method sends the render
	list and viewport to each of the layers such that they
	may fill it with RenderItems to draw.
*/
void GameWorld::addWorldRenderItemsToRenderList(Game *game)
{
	int state=game->getGameState();
	if (state == GAME_PLAYING_GAME_STATE 
		|| game->getShowLevel() 
		|| (state>=PAUSE_GAME_STATE && state<EXIT_GAME_STATE && state!=CONFIRM_DIALOG_STATE))
	{
		GameGraphics *graphics = game->getGraphics();
		RenderList *renderList = graphics->getWorkingWorldRenderList();
		for (int i = 0; i < layers->size(); i++)
		{
			layers->at(i)->addRenderItemsToRenderList(	renderList,
													viewport);
		}
		spriteManager->addSpriteItemsToRenderList(renderList, viewport);
	}
}

/*
	addLevelFileName - This method adds a level file name to the vector
	of all the level file names. Storing these file names allows us to
	easily load a desired level at any time.
*/
void GameWorld::addLevelFileName(wchar_t *levelFileName)
{
	levelFileNames->push_back(levelFileName);
}

/*
	clear - This method removes all data from the World. It should
	be called first when a level is unloaded or changed. If it
	is not called, an application runs the risk of having lots
	of extra data sitting around that may slow the progam down.
	Or, if the world thinks a level is still active, it might add
	items to the render list using image ids that have already been
	cleared from the GameGraphics' texture manager for the world.
	That would likely result in an exception.
*/
void GameWorld::unloadCurrentLevel(Game *game)
{
	spriteManager->clearSprites();
	for(int i =0; i<layers->size(); i++){
		delete layers->at(i);
	}
	layers->clear();	
	game->getAI()->clear();
	currentLevel = 0;
	worldWidth = 0;
	worldHeight = 0;
}

/*
	getLevelFileName - This method gets the name of the file used to
	load the current level. Each level has a file where the layout
	of the level and what artwork to use is specified.
*/
wchar_t* GameWorld::getLevelFileName(int levelNumber)
{
	return levelFileNames->at(levelNumber-1);
}

/*
	loadCurrentLevel - This method changes the current level. This method should
	be called before loading all the data from a level file.
*/
void GameWorld::loadCurrentLevel(Game *game, int initLevel)
{
	if ((initLevel > 0) && (initLevel <= levelFileNames->size()))
	{
		unloadCurrentLevel(game);
		currentLevel = initLevel;
		GameDataLoader *dataLoader = game->getDataLoader();
		dataLoader->loadWorld(game);
	}
}

/*
	update - This method should be called once per frame. It updates
	all of the dynamic level data, like sprite animation states and
	particle locations.
*/
void GameWorld::update(Game *game)
{
	/*viewport->checkAutoScroll(worldWidth,worldHeight);
	if(!viewport->getAutoScrollX()) {
		game->changeGameState(GAME_OVER_GAME_STATE);
	}*/
	spriteManager->updateAllSprites();
	if(spriteManager->getPlayer()!=NULL) {
		GameWorld* gw=game->getWorld();
		AnimatedSprite* player=gw->getSpriteManager()->getPlayer();
		Viewport* v=gw->getViewport();
		int vWidth=gw->getViewport()->getViewportWidth();
		double pct=(double)(v->getViewportX()+vWidth-player->getPhysicalProperties()->getX())/vWidth;
		float playerVx=player->getPhysicalProperties()->getVelocityX();
		if(pct<THRESHOLD && playerVx>0) {
			int thresholdx=THRESHOLD*vWidth+v->getViewportX();
			v->moveViewport(playerVx,0,gw->getWorldWidth(),gw->getWorldHeight());
		} else if(pct>1-THRESHOLD && playerVx<0) {
			int thresholdx=THRESHOLD*vWidth+v->getViewportX();
			v->moveViewport(playerVx,0,gw->getWorldWidth(),gw->getWorldHeight());
		}

		int vHeight=gw->getViewport()->getViewportHeight();
		double pct2=(double)(v->getViewportY()+vHeight-player->getPhysicalProperties()->getY())/vHeight;
		float playerVy=player->getPhysicalProperties()->getVelocityY();
		if(pct2<THRESHOLD && playerVy>0) {
			int thresholdy=THRESHOLD*vHeight+v->getViewportY();
			v->moveViewport(0,playerVy,gw->getWorldWidth(),gw->getWorldHeight());
		} else if(pct2>1-THRESHOLD && playerVy<0) {
			int thresholdx=THRESHOLD*vHeight+v->getViewportY();
			v->moveViewport(0,playerVy,gw->getWorldWidth(),gw->getWorldHeight());
		}
	}
}