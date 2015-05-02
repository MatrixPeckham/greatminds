#pragma once
#include "Game.h"
#include "MouseInput.h"
#include "GameInput.h"
#include "SpriteManager.h"
#include "Player.h"
#include "BoundingVolume.h"
#include "AnimationStates.h"
#include "KeyHandler.h"
#include "GameMusic.h"
#include <math.h>
#define CURSOR_HEIGHT 44
#define RIGHT_CLICK 1
#define LEFT_CLICK 0
void MouseInput::gameInput(int i, Game*g) {
	
	if(i==LEFT_CLICK) {
		if(g->getGameState()==GAME_PLAYING_GAME_STATE) {
			bool right=((KeyHandler*)g->getInput()->getKeyListner())->getFacingRight();;
			BoundingVolume* p=g->getWorld()->getSpriteManager()->getPlayer()->getBoundingVolume();
			Viewport * vp = g->getWorld()->getViewport();
			int offset=g->getWorld()->getViewport()->getViewportOffsetY();
//			float angle=atanf((mousePoint->y-(p->getY()+p->getHeight()/2.0)-offset+CURSOR_HEIGHT/2.0)/(mousePoint->x-(p->getX()+p->getWidth()/2.0)));
			float Y = (((float)mousePoint->y+vp->getViewportY())-offset+CURSOR_HEIGHT/2.0)-((float)p->getY()+p->getHeight()/2.0);
			float X = (((float)mousePoint->x+vp->getViewportX())+CURSOR_HEIGHT/2.0)-((float)p->getX()+p->getWidth()/2.0);
			float mag = sqrt(X*X+Y*Y);
			float unitX = X/mag;
			float unitY = Y/mag;
			float angle=0;
			if(mousePoint->x-p->getX()+vp->getViewportX()<0) {
				right=false;
				g->getWorld()->getSpriteManager()->getPlayer()->setCurrentState(STDL);
			} else {
				right=true;
				g->getWorld()->getSpriteManager()->getPlayer()->setCurrentState(STDR);
			}
				((KeyHandler*)g->getInput()->getKeyListner())->setFacingRight(right);

			((Player*)g->getWorld()->getSpriteManager()->getPlayer())->fire(g, angle, right,unitX,unitY);

		}
		//g->changeGameState(PAUSE_GAME_STATE);
	} else if(i==RIGHT_CLICK) {
		if(g->getGameState()==GAME_PLAYING_GAME_STATE) {
			((GameMusic*)g->getGameAudio())->playEffect("click");
			((Player*)g->getWorld()->getSpriteManager()->getPlayer())->switchWeapon();
		}
	}
}