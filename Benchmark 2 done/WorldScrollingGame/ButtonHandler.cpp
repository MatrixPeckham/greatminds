#pragma once 
#include "ButtonHandler.h"
#include "GameDataLoader.h"
#include "Viewport.h"
#include "GameWorld.h"
#include "AnimatedSprite.h"
#include "Spritemanager.h"
#include "PhysicalProperties.h"
#include "GameMusic.h"
#include "World.h"

extern int GAME_MODE;
void resetLevel(Game* g);

ButtonHandler::ButtonHandler(){}
ButtonHandler::~ButtonHandler(){}
void ButtonHandler::handleButtonEvents(Game* g, wchar_t* cmd) {
	g->setShowLevel(false);
	if(wcscmp(cmd,START)==0) {
		g->changeGameState(GAME_SELECT_STATE);
	}else if(wcscmp(cmd,CTF)==0) {
		GAME_MODE=0;
		g->changeGameState(GAME_PLAYING_GAME_STATE);
	}else if(wcscmp(cmd,ONE)==0) {
		GAME_MODE=1;
		g->changeGameState(GAME_PLAYING_GAME_STATE);
	}else if(wcscmp(cmd,RESUME)==0) {
		g->changeGameState(GAME_PLAYING_GAME_STATE);
	}else if(wcscmp(cmd,PAUSE)==0) {
		g->changeGameState(PAUSE_GAME_STATE);
	}  else if(wcscmp(cmd,QUIT)==0) {
		com=QUIT;
		lastState=g->getGameState();
		g->changeGameState(CONFIRM_DIALOG_STATE);
		g->setShowLevel(true);
	}  else if(wcscmp(cmd,MAINMENU)==0) {
		g->changeGameState(SPLASH_SCREEN_GAME_STATE);
		((GameMusic*)g->getGameAudio())->playNewSong("menu");
	}else if(wcscmp(cmd,EXIT)==0) {
		com=EXIT;
		lastState=SPLASH_SCREEN_GAME_STATE;
		g->changeGameState(CONFIRM_DIALOG_STATE);
	}else if(wcscmp(cmd,ABOUT)==0) {
		lastState=g->getGameState();
		g->changeGameState(ABOUT_GAME_STATE);
	}else if(wcscmp(cmd,HELP)==0) {
		lastState=g->getGameState();
		g->changeGameState(HELP_GAME_STATE);
	} else if(wcscmp(cmd,CTRL)==0) {
		lastState=g->getGameState();
		g->changeGameState(CTRL_GAME_STATE);
	}  else if(wcscmp(cmd,NEW)==0){
		com=NEW;
		lastState=g->getGameState();
		g->changeGameState(NEW_DIALOG_STATE);
		g->setShowLevel(true);
	}else if(wcscmp(cmd,OK)==0) {
		if(wcscmp(com,EXIT)==0) {
			g->changeGameState(EXIT_GAME_STATE);
		} else if(wcscmp(com,QUIT)==0) {
			g->changeGameState(SPLASH_SCREEN_GAME_STATE);
			((GameMusic*)g->getGameAudio())->playNewSong("menu");
		} else if(wcscmp(com,NEW)==0) {
			g->getWorld()->setCurrentLevel(0);
			g->changeGameState(GAME_PLAYING_GAME_STATE);
		}
	} else if(wcscmp(cmd,CANCEL)==0) {
		g->changeGameState(lastState);
	} else if(wcscmp(cmd,BACK)==0){
		g->changeGameState(lastState);
	}
	

}
void resetLevel(Game* g) {
	GameWorld* world=g->getWorld();
	Viewport* viewport=world->getViewport();
	viewport->setViewportX(0);
	viewport->setViewportY(0);
	viewport->setAutoScroll(VIEWPORT_AUTOSCROLL_X, VIEWPORT_AUTOSCROLL_Y);
	viewport->setScrollSpeedX(VIEWPORT_XSPEED);
	viewport->setScrollSpeedY(VIEWPORT_YSPEED);
}
