#pragma once
#include "KeyHandler.h"
#include "GameInput.h"
#include "GameGraphics.h"
#include "SpriteManager.h"
#include "Constants.h"
#include "Player.h"
#include "SparseLayer.h"
#include "GameMusic.h"
void moveRight(Game *g);
void moveLeft(Game *g);
void jump(Game *g);
void duck(Game *g);
void switchPlayer(Game*g);

KeyHandler::~KeyHandler(){}
KeyHandler::KeyHandler(){facingRight=true;}
void KeyHandler::handleKeyEvents(Game* g) {
	
	GameInput* input=g->getInput();
	const unsigned int VK_ESC=27;
	const unsigned int VK_A='A';//37
	const unsigned int VK_D='D';//39
	const unsigned int VK_W='W';//38
	const unsigned int VK_S='S';//40;
	const unsigned int VK_1='1';
	const unsigned int VK_2='2';
	const unsigned int VK_3='3';
	const unsigned int VK_4='4';
	const unsigned int VK_E='E';
	const unsigned int SPACE=32;

	bool key=false;
	SpriteManager* sm=g->getWorld()->getSpriteManager();
	if(input->isKeyDownForFirstTime(VK_1)){
		g->getWorld()->setCurrentLevel(0);
		g->changeGameState(LEVEL_ALMOST_LOADING_MODE);
	} else if(input->isKeyDownForFirstTime(VK_2)){
		g->getWorld()->setCurrentLevel(1);
		g->changeGameState(LEVEL_ALMOST_LOADING_MODE);
	} else if(input->isKeyDownForFirstTime(VK_3)){
		g->getWorld()->setCurrentLevel(2);
		g->changeGameState(LEVEL_ALMOST_LOADING_MODE);
	} else if(input->isKeyDownForFirstTime(VK_4)){
		g->getWorld()->setCurrentLevel(3);
		g->changeGameState(LEVEL_ALMOST_LOADING_MODE);
	}
	//Check game state
	if(g->getGameState()==GAME_PLAYING_GAME_STATE){                                                                                                                                                                                                       
		int targetState=0;
		if(!facingRight) {
			targetState=1;
		}
		float vx=sm->getPlayer()->getPhysicalProperties()->getVelocityX();
		float x=sm->getPlayer()->getPhysicalProperties()->getX();
		float y=sm->getPlayer()->getPhysicalProperties()->getY();
		float ww=g->getWorld()->getWorldWidth();
		float wh=g->getWorld()->getWorldHeight();
		//int pw=sm->getPlayer()->getBoundingVolume()->getWidth();
		if(input->isKeyDown(VK_W)) {
			key=true;
			if(input->isKeyDownForFirstTime(VK_W)) {
						((GameMusic*)g->getGameAudio())->stopEffect("walk");
						((GameMusic*)g->getGameAudio())->playEffect("walk");
					}
			targetState = UP;
				if(y>0) {
					
						sm->getPlayer()->getPhysicalProperties()->setVelocityY(-1*JUMP);
				} else {
					sm->getPlayer()->getPhysicalProperties()->setY(0);
				}
		} else if(input->isKeyDown(VK_S)) {
			if(input->isKeyDownForFirstTime(VK_S)) {
						((GameMusic*)g->getGameAudio())->stopEffect("walk");
						((GameMusic*)g->getGameAudio())->playEffect("walk");
					}
			key=true;
			targetState = DOWN;

			if(y<wh-64) {
							sm->getPlayer()->getPhysicalProperties()->setVelocityY(JUMP);
			} else {
				sm->getPlayer()->getPhysicalProperties()->setY(wh-64);
			}
		} else {
				sm->getPlayer()->getPhysicalProperties()->setVelocityY(0);
		}

		if(input->isKeyDown(VK_A)) {
			key=true;
			facingRight=false;
			targetState=ML;//move left
				if(x>0) {
					if(input->isKeyDownForFirstTime(VK_A)) {
						((GameMusic*)g->getGameAudio())->stopEffect("walk");
						((GameMusic*)g->getGameAudio())->playEffect("walk");
					}
						sm->getPlayer()->getPhysicalProperties()->setVelocityX(-1*RUNSPEED);
					//}
				} else {
					sm->getPlayer()->getPhysicalProperties()->setX(0);
				}
		} else if(input->isKeyDown(VK_D)) {
			if(input->isKeyDownForFirstTime(VK_D)) {
						((GameMusic*)g->getGameAudio())->stopEffect("walk");
						((GameMusic*)g->getGameAudio())->playEffect("walk");
			}
			key=true;
			facingRight=true;
			targetState=MR;//moveRight
			if(x<ww-64) {
					//if(input->isKeyDownForFirstTime(VK_D)) {
						sm->getPlayer()->getPhysicalProperties()->setVelocityX(RUNSPEED);
						
				//}
			} else {
				sm->getPlayer()->getPhysicalProperties()->setVelocityX(0);
			}
		} else {
			sm->getPlayer()->getPhysicalProperties()->setVelocityX(0);
		}
		
		if(targetState!=sm->getPlayer()->getCurrentState()) {
			sm->getPlayer()->setCurrentState(targetState);
		}
		if(input->isKeyDown(VK_ESC)) {
			g->changeGameState(PAUSE_GAME_STATE);

		}
		if(g->getGameState()!=GAME_PLAYING_GAME_STATE) {
			((GameMusic*)g->getGameAudio())->stopEffect("walk");
		}
		if(!key) {
			((GameMusic*)g->getGameAudio())->stopEffect("walk");
		}
		if(input->isKeyDown(SPACE) && ((Player *)g->getWorld()->getSpriteManager()->getPlayer())->canTakeFlag()) {
			GameWorld* world=g->getWorld();
			Player* p=(Player*) world->getSpriteManager()->getPlayer();
			p->hasMessage=true;
			OverlayImage* t=((SparseLayer*)world->getLayers()->at(1))->getTile(1);
			t->x=p->getBoundingVolume()->getX();
			t->y=p->getBoundingVolume()->getY();
		} else {
			GameWorld* world=g->getWorld();
			Player* p=(Player*) world->getSpriteManager()->getPlayer();
			p->hasMessage=false;
		}

	}
	
}
void switchPlayer(Game* g) {
	SpriteManager* sm=g->getWorld()->getSpriteManager();
	
}
