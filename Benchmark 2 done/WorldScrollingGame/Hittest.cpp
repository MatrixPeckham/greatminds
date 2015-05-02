#pragma once 
#include <vector>
#include "HitTest.h"
#include "Game.h"
#include "TiledLayer.h"
#include "CollidableObject.h"
#include "Projectile.h"
#include "Player.h"
#include "PhysicalProperties.h"
#include "GameWorld.h"
#include "WorldLayer.h"
#include "TiledLayer.h"
#include "Constants.h"
#include "GameInput.h"

HitTest::HitTest() {
	ptr=0;
	applyG=true;
	collisions=new collision*[MAX_COLLISIONS];
	for(int i=0; i< MAX_COLLISIONS; i++) {
		collisions[i]=new collision();
		AnimatedSprite* o2=new AnimatedSprite();
		PhysicalProperties* pp2=new PhysicalProperties();
		BoundingVolume* v=new BoundingVolume();
		o2->setBoundingVolume(v);
		o2->setPhysicalProperties(pp2);
		collisions[i]->o2=o2;

	}
}
HitTest::~HitTest() {
	clear();
	delete collisions;
}
void HitTest::clear() {
	int i=0;
	ptr=0;
	while(i<MAX_COLLISIONS) {
		delete collisions[i];
		i++;
	}
}
void HitTest::reset() {
	ptr=0;
}
bool HitTest::hitTest(CollidableObject* obj1, CollidableObject* obj2) {
	return checkXCollide(obj1,obj2) && checkYCollide(obj1,obj2);
}
float HitTest::getFirstTimeX(CollidableObject* obj1, CollidableObject* obj2) {
	//get the newtonian reference frame
	float netVx=obj1->getPhysicalProperties()->getVelocityX()-obj2->getPhysicalProperties()->getVelocityX();//object 1's velocity in relation to 
	float x1=obj1->getBoundingVolume()->getX();
	float x2=obj2->getBoundingVolume()->getX();
	float x1w=obj1->getBoundingVolume()->getWidth();
	float x2w=obj2->getBoundingVolume()->getWidth();
	float time;
	
	if(netVx<0) {
		float f=((x2+x2w)-x1)/netVx;
		return f;
	}

	if(netVx>0) {
		float f=(x2-(x1+x1w))/netVx;
		return f;

		
	} 
	if(checkXCollide(obj1,obj2)) {
		return 0;
	}

	return 2;		
}
float HitTest::getFirstTimeY(CollidableObject* obj1, CollidableObject* obj2) {
	//get the newtonian reference frame
	float netVy=obj1->getPhysicalProperties()->getVelocityY()-obj2->getPhysicalProperties()->getVelocityY();//object 1's velocity in relation to 
	float y1=obj1->getBoundingVolume()->getY();
	float y2=obj2->getBoundingVolume()->getY();
	float y2h=obj2->getBoundingVolume()->getHeight();
	float y1h=obj1->getBoundingVolume()->getHeight();
	float time;
	if(netVy<0) {
		float f=-1*(y1-(y2+y2h))/netVy;
		return f;
	}

	if(netVy>0) {
		float f=(y2-(y1+y1h))/netVy;

		return f;
		
	} 
	if(netVy==0 && checkYCollide(obj1,obj2)) {
		return 0;
	}
	return 2;		
}
float HitTest::getLastTimeX(CollidableObject* obj1, CollidableObject* obj2) {
	float netVx=obj1->getPhysicalProperties()->getVelocityX()-obj2->getPhysicalProperties()->getVelocityX();//object 1's velocity in relation to 
	float x1=obj1->getBoundingVolume()->getX();
	float x2=obj2->getBoundingVolume()->getX();
	float x1w=obj1->getBoundingVolume()->getWidth();
	float x2w=obj2->getBoundingVolume()->getWidth();
	float time;
	
	if(netVx<0) {
		float f=(x2-(x1+x1w))/netVx;
		return f;
	}

	if(netVx>0) {
		float f=((x2+x2w)-(x1))/netVx;
		return f;

		
	} 

	return 2;	
}
float HitTest::getLastTimeY(CollidableObject* obj1, CollidableObject* obj2) {
	//get the newtonian reference frame
	float netVy=obj1->getPhysicalProperties()->getVelocityY()-obj2->getPhysicalProperties()->getVelocityY();//object 1's velocity in relation to 
	float y1=obj1->getBoundingVolume()->getY();
	float y2=obj2->getBoundingVolume()->getY();
	float y2h=obj2->getBoundingVolume()->getHeight();
	float y1h=obj1->getBoundingVolume()->getHeight();
	float time;
	if(netVy<0) {
		float f=-1*((y1+y1h)-y2)/netVy;
		return f;
	}

	if(netVy>0) {
		float f=((y2+y2h)-(y1))/netVy;

		return f;
		
	} 

	return 2;
}
void HitTest::checkAllTileCollisions(Game*g, AnimatedSprite* obj){
	//move x && y to end of frame
	 
	if(!obj->isCurrentlyCollidable()) {
		return;
	}
	TiledLayer* t=(TiledLayer*) g->getWorld()->getLayers()->at(0);
	float x=obj->getBoundingVolume()->getX();
	float y=obj->getBoundingVolume()->getY();
	float xs=obj->getPhysicalProperties()->getVelocityX();
	float ys=obj->getPhysicalProperties()->getVelocityY();
	int mincol=	(int)(x/t->getTileWidth())-1;
	int minrow=(int)(y/t->getTileHeight())-1;
	x=obj->getBoundingVolume()->getX();
	y=obj->getBoundingVolume()->getY();
	float width=obj->getBoundingVolume()->getWidth();
	float height=obj->getBoundingVolume()->getHeight();
	if(xs>=0) {
		x+=obj->getBoundingVolume()->getWidth();
	}
	if(ys>=0) {
		y+=obj->getBoundingVolume()->getHeight();
	}
	int maxcol=(int)((x+xs+width)/t->getTileWidth());
	int maxrow=(int)((y+ys+height)/t->getTileHeight());
	if(mincol>maxcol) {
		int tmp=maxcol;
		maxcol=mincol;
		mincol=tmp;
	}
	if(minrow>maxrow) {
		int tmp=maxrow;
		maxrow=minrow;
		minrow=tmp;
	}
	if(minrow<0) {
		minrow=0;
	}
	if(mincol<0) {
		mincol=0;
	}
	if(maxrow>=t->getRows()) {
		maxrow=t->getRows()-1;
	}
	if(maxcol>=t->getColumns()) {
		maxcol=t->getColumns()-1;
	}
	
	
	
	
	//Convert Tiles to collidable objects for checking
	while(mincol<=maxcol) {
		int r=minrow;
		while(r<=maxrow) {
			if(t->getTile(r,mincol)->collidable) {
				AnimatedSprite* tmp=collisions[ptr]->o2;
				PhysicalProperties* pp=tmp->getPhysicalProperties();
				BoundingVolume* v=tmp->getBoundingVolume();
				pp->setX(mincol*t->getTileWidth());
				pp->setY(r*t->getTileHeight());
				v->setX(mincol*t->getTileWidth());
				v->setY(r*t->getTileHeight());
				v->setHeight(t->getTileHeight());
				v->setWidth(t->getTileWidth());
				pp->setVelocityX(0);
				pp->setVelocityY(0);
				tmp->setBoundingVolume(v);
				tmp->setPhysicalProperties(pp);

				float time=calculateHitTime(obj, tmp);
				obj->getPhysicalProperties()->setAccelerationY(GRAVITY);
				if(time<1 && time>=0 || hitTest(obj,tmp)) {
					collisions[ptr]->firstTime=time;
					collisions[ptr]->o1=obj;
					collisions[ptr]->o2=tmp;
					ptr++;
				} 
					
			}
			r++;
		}
		mincol++;
	}
}
float HitTest::calculateHitTime(CollidableObject* obj1, CollidableObject* obj2) {
	
	float sy=this->getFirstTimeY(obj1, obj2);
	float fy=this->getLastTimeY(obj1, obj2);
	float sx=this->getFirstTimeX(obj1, obj2);
	float fx=this->getLastTimeX(obj1, obj2);

	collisions[ptr]->firstXTime=sx;
	collisions[ptr]->lastXTime=fx;
	collisions[ptr]->firstYTime=sy;
	collisions[ptr]->lastYTime=fy;
	collisions[ptr]->right=0;
	collisions[ptr]->top=0;
	if(sy==sx) {
		if(sy!=2) {
		collisions[ptr]->top=1;
		collisions[ptr]->right=1;
		}
		return sy;
	}else if(sy>=sx && sy<=fx) {
		collisions[ptr]->top=1;
		return sy;
	} else if(sx>=sy && sx<=fy) {
		collisions[ptr]->right=1;
		return sx;
	} else {
		//no overlap
		return 2;
	}

}
void HitTest::sort() {
	int lastIndex=ptr;
     buildHeap();//first make it a heap
     while(lastIndex>0) {
            heapify(0, lastIndex);
            collision* tmp=collisions[0];
            collisions[0]=collisions[lastIndex];
            collisions[lastIndex]=tmp;
            lastIndex--;        
     }
}
void HitTest::checkAllSpriteCollisions(Game*g, AnimatedSprite* obj1, AnimatedSprite* obj2) {
	float time=calculateHitTime(obj1, obj2);
				if(time<1 && time>=0) {
					((Player*)obj2)->hp-=5;
				} 
}
void HitTest::heapify(int index, int lastIndex) {
	collision* tmp;
     int i=index;
	 
     while(i<=lastIndex) {
                if(2*i+2>lastIndex || 2*i+1>lastIndex) {
					if(2*i+1<=lastIndex && collisions[i]<collisions[2*i+1]) {
						tmp=collisions[i];
						collisions[i]=collisions[2*i+1];
						collisions[2*i+1]=tmp;
						i=2*i+1;
					} else {
						break;
					}
				} else if(collisions[i]<collisions[2*i+1] && collisions[i]<collisions[2*i+2]) {
					if(collisions[2*i+2]>collisions[2*i+1]) {
						tmp=collisions[i];
						collisions[i]=collisions[2*i+2];
						collisions[2*i+2]=tmp;
						i=2*i+2;
					} else {
						tmp=collisions[i];
						collisions[i]=collisions[2*i+1];
						collisions[2*i+1]=tmp;
						i=2*i+1;
					}
				} else if(collisions[i]<collisions[2*i+1]) {
					tmp=collisions[i];
					collisions[i]=collisions[2*i+1];
					collisions[2*i+1]=tmp;
					i=2*i+1;
				} else if(collisions[i]<collisions[2*i+2]) {
					tmp=collisions[i];
					collisions[i]=collisions[2*i+2];
					collisions[2*i+2]=tmp;
					i=2*i+2;
				} else {
					break;
				}
            }
	
}
void HitTest::buildHeap() {
	int LAST_INDEX=ptr;
	int index=LAST_INDEX;

		while(index>=0) {
			heapify(index, LAST_INDEX);
			index--;
		}
}

void HitTest::invert() {
	for(int i=0; i<ptr/2; i++) {
		collision *tmp=collisions[i];
		collisions[i]=collisions[ptr-i-1];
		collisions[ptr-i-1]=tmp;
	}
}
void HitTest::handle(int i) {
	if(collisions[i]->x) {
		handleCollisionX(i);
	} else {
		handleCollisionY(i);
	}
}
bool HitTest::checkXCollide(CollidableObject* obj1, CollidableObject* obj2) {
	float minx1=obj1->getBoundingVolume()->getX();
	float maxx1=obj1->getBoundingVolume()->getX()+obj1->getBoundingVolume()->getWidth();
	float minx2=obj2->getBoundingVolume()->getX();
	float maxx2=obj2->getBoundingVolume()->getX()+obj2->getBoundingVolume()->getWidth();
	if((minx1>=minx2 && minx1<=maxx2) || (minx2>=minx1 && minx2<=maxx1)) {
		return true;
	}
	return false;
}
bool HitTest::checkYCollide(CollidableObject* obj1, CollidableObject* obj2) {
	float miny1=obj1->getBoundingVolume()->getY();
	float mayy1=obj1->getBoundingVolume()->getY()+obj1->getBoundingVolume()->getHeight();
	float miny2=obj2->getBoundingVolume()->getY();
	float mayy2=obj2->getBoundingVolume()->getY()+obj2->getBoundingVolume()->getHeight();
	if((miny1>miny2 && miny1<mayy2) || (mayy1>miny2 && mayy1<mayy2)) {
		return true;
	}
	return false;
}
collision* HitTest::get(int i) {
	return collisions[i];
}
void HitTest::handleCollisionY(int i) {
	Projectile* p=((Projectile*)collisions[i]->o1);
	switch(collisions[i]->o1->getSpriteType()->getSpriteTypeID()) {
		case 100:
			
			p->dead=true;
			p->setXY(-1000,-1000);
			break;
		case 101:
			p->dead=true;
			p->setXY(-1000,-1000);
			break;
		default:
	float pctY=collisions[i]->firstYTime;
	float pctX=collisions[i]->firstXTime;
	PhysicalProperties* pp=collisions[i]->o1->getPhysicalProperties();
	PhysicalProperties* pp2=collisions[i]->o2->getPhysicalProperties();
	BoundingVolume* bv=collisions[i]->o1->getBoundingVolume();
	BoundingVolume* bv2=collisions[i]->o2->getBoundingVolume();
	float netVy=pp->getVelocityY();
	
	if(collisions[i]->top==1) {
		if(netVy>0) {
			bv->setY(bv2->getY()-bv->getHeight()-1);
			pp->setY(bv->getY());
		} else if(netVy<0) {
			bv->setY(bv2->getY()+bv2->getHeight()+1);
			pp->setY(bv->getY());
			
		}
		pp->setVelocityY(0);
	}
	}

					
}
void HitTest::handleCollisionX(int i) {
	//for the tiles obj2 is a tile
	Projectile* p=((Projectile*)collisions[i]->o1);
	switch(collisions[i]->o1->getSpriteType()->getSpriteTypeID()) {
		case 100:
			
			p->dead=true;
			p->setXY(-1000,-1000);
			break;
		case 101:
			p->dead=true;
			p->setXY(-1000,-1000);
			break;
		default:
	PhysicalProperties* pp=collisions[i]->o1->getPhysicalProperties();
	PhysicalProperties* pp2=collisions[i]->o2->getPhysicalProperties();
	BoundingVolume* bv=collisions[i]->o1->getBoundingVolume();
	BoundingVolume* bv2=collisions[i]->o2->getBoundingVolume();
	float netVx=pp->getVelocityX();
		if(netVx>0) {
			bv->setX(bv2->getX()-bv->getWidth()-1);
			pp->setX(bv->getX());
			
		} else if(netVx<0) {
			bv->setX(bv2->getX()+bv2->getWidth()+1);
			pp->setX(pp->getX());
		}
		pp->setVelocityX(0);	
	}
}