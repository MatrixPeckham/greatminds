#pragma once
#include "MyPhysics.h"
#include "HitTest.h"
#include "SpriteManager.h"
#include "Game.h"
#include "Projectile.h"
#include "SparseLayer.h"
#include "PhysicalProperties.h"
#include "TiledLayer.h"
#include "BoundingVolume.h"
#include "Player.h"
#include "GameMusic.h"
#include "GameWorld.h"
#include "Viewport.h"
#include "Bot.h"
#include "GameAI.h"
#define BASEID 0
extern int GAME_MODE;
MyPhysics::MyPhysics() {
	h=new HitTest();
	needExplosion = true;
	tmp=new CollidableObject();
	pp=new PhysicalProperties();
	bv=new BoundingVolume();
}
void MyPhysics::update(Game *g) {
	
	h->reset();
	if(GAME_MODE==0) {
		messageHit(g);
	}
	baseHit(g);
		
	SpriteManager* sm=g->getWorld()->getSpriteManager();
	h->checkAllTileCollisions(g, sm->getPlayer());
	handleCollide(g);
	int size= g->getAI()->getNumBots();
	for(int j=0; j<size; j++) {
		Bot* b=g->getAI()->getBotAt(j);
		if(b->isDead()) {
			continue;
		}
		for(int i=0; i<sm->getNumberOfSprites(); i++) {
			int id=sm->getSprite(i)->getSpriteType()->getSpriteTypeID();
			if((id == 100 || id == 101) && ((Projectile*)sm->getSprite(i))->dead==false) {
				h->checkAllTileCollisions(g, sm->getSprite(i));
				handleCollide(g);
				float time=h->calculateHitTime(b->getAnimatedSprite(),sm->getSprite(i));
				if(time>=0 && time<1) {

					((Player*)b->getAnimatedSprite())->hp-=5;
					((AIPlayer*)b->getAnimatedSprite())->setHit(true);
					if(id == 100)
					{
						((Player*)b->getAnimatedSprite())->hp-=5;
						exp->getPhysicalProperties()->setX(sm->getSprite(i)->getPhysicalProperties()->getX());
						exp->getPhysicalProperties()->setY(sm->getSprite(i)->getPhysicalProperties()->getY());
						exp->setCurrentState(0);
						((GameMusic*)g->getGameAudio())->playEffect("bombExplode");
					}
					Projectile* p=((Projectile*)sm->getSprite(i));
					p->setXY(-10000,-10000);
					p->getPhysicalProperties()->setVelocityX(0);
					p->getPhysicalProperties()->setVelocityY(0);
					p->dead=true;
					/*if(((Player*)sm->getPlayer())->hp==0) {
						g->changeGameState(GAME_OVER_GAME_STATE);
					}*/
				}
			}
		}
	}
	for(int i=0; i<sm->getNumberOfSprites(); i++) {
		if(sm->getSprite(i)->getSpriteType()->getSpriteTypeID() == 4444)
		{
			if(sm->getSprite(i)->getFrameIndex() == 6)
			{
				sm->getSprite(i)->setCurrentState(1);
				((Explosion*)sm->getSprite(i))->setDead(true);
			}

			/*else
			{
				float x = sm->getSprite(i)->getPhysicalProperties()->getX();
				float y = sm->getSprite(i)->getPhysicalProperties()->getY();

				float vx = g->getWorld()->getViewport()->getViewportX();
				float vy = g->getWorld()->getViewport()->getViewportX();
				float vw = g->getWorld()->getViewport()->getViewportWidth();
				float vh =g->getWorld()->getViewport()->getViewportHeight();
					
				if(x < vx || x > vx+vw && y < vy && y > vy+vh)
				{
					sm->getSprite(i)->setCurrentState(1);
					((Explosion*)sm->getSprite(i))->setDead(true);
				}
			}*/

			if(needExplosion == true)
			{
				exp = (Explosion*)sm->getSprite(i);
				exp->getPhysicalProperties()->setX(sm->getSprite(i)->getPhysicalProperties()->getX());
				exp->getPhysicalProperties()->setY(sm->getSprite(i)->getPhysicalProperties()->getY());
				exp->getPhysicalProperties()->setAccelerationX(0);
				exp->getPhysicalProperties()->setAccelerationY(0);
				exp->getPhysicalProperties()->setVelocityX(0);
				exp->getPhysicalProperties()->setVelocityY(0);

				if(exp->dead == true)
				{
					this->needExplosion = false;
					exp->setDead(false);
				}
			}
		}
		else if(((Projectile*)sm->getSprite(i))->dead==false) {
			h->checkAllTileCollisions(g, sm->getSprite(i));
			handleCollide(g);
			float time=h->calculateHitTime(sm->getPlayer(),sm->getSprite(i));
			if(time>=0 && time<1) {

				((Player*)sm->getPlayer())->hp-=5;

				if(((Player*)sm->getPlayer())->getCurrentState() == 1 || ((Player*)sm->getPlayer())->getCurrentState() == 3)
				{
					((Player*)sm->getPlayer())->setCurrentState(11);
					((Player*)sm->getPlayer())->changeFrame();
				}
				else if(((Player*)sm->getPlayer())->getCurrentState() == 0 || ((Player*)sm->getPlayer())->getCurrentState() == 2)
				{
					((Player*)sm->getPlayer())->setCurrentState(10);
					((Player*)sm->getPlayer())->changeFrame();
				}

				((GameMusic*)g->getGameAudio())->playEffect("hit");
				((Player*)sm->getPlayer())->getPhysicalProperties()->setVelocityX(0);
				((Player*)sm->getPlayer())->getPhysicalProperties()->setVelocityY(0);
				if(sm->getSprite(i)->getSpriteType()->getSpriteTypeID() == 100)
				{
					((Player*)sm->getPlayer())->hp-=5;
					exp->getPhysicalProperties()->setX(sm->getSprite(i)->getPhysicalProperties()->getX());
					exp->getPhysicalProperties()->setY(sm->getSprite(i)->getPhysicalProperties()->getY());
					exp->setCurrentState(0);
					exp->setDead(false);
					this->needExplosion = true;
					((GameMusic*)g->getGameAudio())->playEffect("bombExplode");
				}
				else if(sm->getSprite(i)->getSpriteType()->getSpriteTypeID() == 101)
				{
					exp->getPhysicalProperties()->setX(sm->getSprite(i)->getPhysicalProperties()->getX());
					exp->getPhysicalProperties()->setY(sm->getSprite(i)->getPhysicalProperties()->getY());
					exp->setCurrentState(0);
					exp->setDead(false);
					this->needExplosion = true;
					((GameMusic*)g->getGameAudio())->playEffect("bulletBlock");
				}
				Projectile* p=((Projectile*)sm->getSprite(i));
				((Player*)sm->getPlayer())->getPhysicalProperties()->setVelocityX(0);
				((Player*)sm->getPlayer())->getPhysicalProperties()->setVelocityY(0);
				p->setXY(-100000,-100000);
				p->getPhysicalProperties()->setVelocityX(0);
				p->getPhysicalProperties()->setVelocityY(0);
				p->dead=true;
				if(((Player*)sm->getPlayer())->hp<=0) {
					sm->getPlayer()->setCurrentState(12);
					((Player*)sm->getPlayer())->changeFrame();
					((GameMusic*)g->getGameAudio())->playEffect("dead");
					((GameMusic*)g->getGameAudio())->playEffect("lose");
					g->changeGameState(GAME_OVER_GAME_STATE);
				}
			}
		}
	}
	

}
void MyPhysics::handleBulletCollide() {
	while(h->size()>0){
		h->sort();
		//bool f=h->isHeap();
		for(int i=0; i<h->size(); i++) {
			//float time=h->calculateHitTime(h->get(i)->o1, h->get(i)->o2);
			//if(h>0 && h<1)  {
			if(h->get(i)->right==1) {
				h->handleCollisionX(i);
			} 
			if(h->get(i)->top==1) {
				h->handleCollisionY(i);;
			}
			//h->checkAllTileCollisions(g, 
			//}
			
		}
		h->reset();		
	}
}
void MyPhysics::handleCollide(Game* g) {
	while(h->size()>0){
		h->sort();
		//bool f=h->isHeap();
		for(int i=0; i<h->size(); i++) {
			//float time=h->calculateHitTime(h->get(i)->o1, h->get(i)->o2);
			//if(h>0 && h<1)  {
			int id=h->collisions[i]->o1->getSpriteType()->getSpriteTypeID();
			Viewport* v=g->getWorld()->getViewport();
			int x=h->collisions[i]->o1->getPhysicalProperties()->getX();
			int y=h->collisions[i]->o1->getPhysicalProperties()->getY();
			bool inframe=x>=v->getViewportX() && x<=v->getViewportWidth()+v->getViewportX() && y>=v->getViewportY() && y<=v->getViewportY()+v->getViewportHeight();
			if(h->get(i)->right==1) {
				
				if(id == 100 || id==101)
				{
					float x = h->collisions[i]->o1->getPhysicalProperties()->getX();
					float y = h->collisions[i]->o1->getPhysicalProperties()->getY();

					float vx = g->getWorld()->getViewport()->getViewportX();
					float vy = g->getWorld()->getViewport()->getViewportY();
					float vw = g->getWorld()->getViewport()->getViewportWidth();
					float vh =g->getWorld()->getViewport()->getViewportHeight();
					
					if(x >= vx && x <= vx+vw && y >= vy && y <= vy+vh)
					{
						exp->getPhysicalProperties()->setX(x);
						exp->getPhysicalProperties()->setY(y);
						exp->setCurrentState(0);
						exp->setDead(false);
						this->needExplosion = true;
						if(inframe) {
							if(id==101) {
								((GameMusic*)g->getGameAudio())->playEffect("bulletBlock");
							} else {
								((GameMusic*)g->getGameAudio())->playEffect("bombExplode");
							}
						}
					}

				}
				h->handleCollisionX(i);
			} 
			if(h->get(i)->top==1) {
				if(id == 100 || id==101)
				{
					float x = h->collisions[i]->o1->getPhysicalProperties()->getX();
					float y = h->collisions[i]->o1->getPhysicalProperties()->getY();

					float vx = g->getWorld()->getViewport()->getViewportX();
					float vy = g->getWorld()->getViewport()->getViewportY();
					float vw = g->getWorld()->getViewport()->getViewportWidth();
					float vh =g->getWorld()->getViewport()->getViewportHeight();
					
					if(x >= vx && x <= vx+vw && y >= vy && y <= vy+vh)
					{
						exp->getPhysicalProperties()->setX(x);
						exp->getPhysicalProperties()->setY(y);
						exp->setCurrentState(0);
						exp->setDead(false);
						this->needExplosion = true;
						if(inframe) {
							if(id==101) {
								((GameMusic*)g->getGameAudio())->playEffect("bulletBlock");
							} else {
								((GameMusic*)g->getGameAudio())->playEffect("bombExplode");
							}
						}
					}

				}
				h->handleCollisionY(i);;
			}
			//h->checkAllTileCollisions(g, 
			//}
			
		}
		h->reset();		
	}
}
void MyPhysics::baseHit(Game* g) {
	int x=((SparseLayer*)g->getWorld()->getLayers()->at(1))->getTile(BASEID)->x;
	int y=((SparseLayer*)g->getWorld()->getLayers()->at(1))->getTile(BASEID)->y;
	int width=((SparseLayer*)g->getWorld()->getLayers()->at(1))->getTile(BASEID)->width;
	int height=((SparseLayer*)g->getWorld()->getLayers()->at(1))->getTile(BASEID)->height;
	bv->setX(x);
	bv->setY(y);
	bv->setWidth(width);
	bv->setHeight(height);
	pp->setX(x);
	pp->setY(y);
	pp->setVelocityX(0);
	pp->setVelocityY(0);
	tmp->setBoundingVolume(bv);
	tmp->setPhysicalProperties(pp);
	SpriteManager* sm=g->getWorld()->getSpriteManager();
	if(h->hitTest(sm->getPlayer(), tmp)) {
		Player * p=(Player*)sm->getPlayer();
		if(p->hasMessage) {
			g->changeGameState(LEVEL_ALMOST_LOADING_MODE);
			((GameMusic*)g->getGameAudio())->playEffect("win");
			
		} else {
			p->ammo=25;
			p->bombs=10;
		}
	} 
}
void MyPhysics::messageHit(Game* g) {
	
	int x=((SparseLayer*)g->getWorld()->getLayers()->at(1))->getTile(1)->x;
	int y=((SparseLayer*)g->getWorld()->getLayers()->at(1))->getTile(1)->y;
	int width=((SparseLayer*)g->getWorld()->getLayers()->at(1))->getTile(1)->width;
	int height=((SparseLayer*)g->getWorld()->getLayers()->at(1))->getTile(1)->height;
	bv->setX(x);
	bv->setY(y);
	bv->setWidth(width);
	bv->setHeight(height);
	pp->setX(x);
	pp->setY(y);
	pp->setVelocityX(0);
	pp->setVelocityY(0);
	tmp->setBoundingVolume(bv);
	tmp->setPhysicalProperties(pp);
	SpriteManager* sm=g->getWorld()->getSpriteManager();
	if(h->hitTest(sm->getPlayer(), tmp)) {
		((Player*)sm->getPlayer())->setTakeFlag(true);
	} else {
		((Player*)sm->getPlayer())->setTakeFlag(false);
	}
}