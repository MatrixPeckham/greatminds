#pragma once
#include "GameAI.h"
#include "Game.h"
#include "Bot.h"
#include "TiledLayer.h"
extern int GAME_MODE;
GameAI::GameAI() {
	bots=new vector<Bot*>();
	
}
void GameAI::init(Game * game){
/*	Bot * b = new Bot();
	TiledLayer * tiles =(TiledLayer *) game->getWorld()->getLayers()->at(0);
	AnimatedSpriteType * ast = new AnimatedSpriteType();
	ast->addAnimationState(constructEmptyWCHAR_TArray(L"state"));
	int id = game->getGraphics()->getWorldTextureManager()->loadTexture(constructEmptyWCHAR_TArray(L"images/Enemies/AI_DUMMY.png"));
	ast->addAnimationFrame(0,id);
	ast->addAnimationFrame(0,id);
	ast->setSpriteTypeID(99);
	ast->setTextureSize(64,64);
	ast->setAnimationSpeed(1);
	AIPlayer * sp = new AIPlayer(game);
	PhysicalProperties * pp = new PhysicalProperties();
	pp->setX(900);
	pp->setY(600);
	pp->setVelocityX(0);
	pp->setVelocityY(0);
	pp->setAccelerationX(0);
	pp->setAccelerationY(0);
	BoundingVolume * bv = new BoundingVolume();
	bv->setX(900);
	bv->setY(600);
	bv->setWidth(64);
	bv->setHeight(64);
	sp->setPhysicalProperties(pp);
	sp->setBoundingVolume(bv);
	sp->setSpriteType(ast);
	sp->setAlpha(255);
	sp->setCurrentState(0);
	SpriteManager * sm = game->getWorld()->getSpriteManager();
	sm->addSpriteType(ast);
	b->setSprite(sp);
	sm->addSprite(sp);
	bots->push_back(b);*/
}
GameAI::~GameAI() {
	delete bots;
}
void GameAI::update(Game* g) {
	airemaining=bots->size();
	for(int i=0; i<bots->size();i++) {
		bots->at(i)->update(g);
		if(((AIPlayer*)bots->at(i)->getAnimatedSprite())->hp<=0) {
			airemaining--;
		}
	}
	if(airemaining==0 && GAME_MODE==1) {
		g->changeGameState(LEVEL_ALMOST_LOADING_MODE);
	}
}
void GameAI::clear(){
	bots->clear();
}