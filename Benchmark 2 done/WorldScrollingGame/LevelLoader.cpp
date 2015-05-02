#pragma once
#include "LevelLoader.h"


LevelLoader::LevelLoader(){}
LevelLoader::~LevelLoader(){}

void LevelLoader::loadGUI(Game *game){
}
void LevelLoader::loadWorld(Game *game){
	/*
	Viewport *viewport = world->getViewport();
	viewport->setViewportWidth(1024);
	viewport->setViewportHeight(704);
	viewport->setViewportOffsetX(0);
	viewport->setViewportOffsetY(64);
	world->setWorldWidth(1920);
	world->setWorldHeight(1280);

	AnimatedSpriteType *ast = new AnimatedSpriteType();
	fileName = constructEmptyWCHAR_TArray(L"./images/bomb.jpg");
	int spriteImageID1 = worldTextureManager->loadTexture(fileName);
	ast->addImageID(spriteImageID1);
	fileName = constructEmptyWCHAR_TArray(L"./cursor",);
	int spriteImageID2 = worldTextureManager->loadTexture(fileName);
	ast->addImageID(spriteImageID2);
	fileName = constructEmptyWCHAR_TArray(L"./images/blueblock.jpg");
	int spriteImageID3 = worldTextureManager->loadTexture(fileName);
	ast->addImageID(spriteImageID3);

	
	ast->setAnimationSpeed(((WindowsGameTimer*)game->getTimer())->getTargetFPS()*2);

	wchar_t *animStateName = constructEmptyWCHAR_TArray(L"STANDING");
	ast->addAnimationState(animStateName);
	vector<int> *standingSequence = new vector<int>();

	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID2);
	ast->addAnimationFrame(0, spriteImageID3);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	*/
}