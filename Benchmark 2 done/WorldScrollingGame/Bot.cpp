#include "stdafx.h"
#include "Bot.h"
#include "GameWorld.h"
#include "SpriteManager.h"
#include "AnimatedSprite.h"
#include "AIPlayer.h"
#include "TiledLayer.h"
#include "GameMusic.h"
#include "constants.h"
#include "GameTimer.h"
#include <vector>
#include <map>

bool operator ==(const Inds &a, const Inds &b){
	return a.x==b.x&&a.y==b.y;
};

bool operator <(const Inds &a, const Inds &b){
	if(a.x==b.x){
		return a.y<b.y;
	}
	return a.x<b.x;
}

int calc_H_Score(Inds &to, Inds &from){
	int sc = (to.x-from.x)*(to.x-from.x)+(to.y-from.y)*(to.y-from.y);
	return sc;
}
int getNeighbors(Inds &cur, TiledLayer * tiles, Inds * neighbors){
	int i = 0;
	int x = cur.x;
	int y = cur.y;
	if(x<0||y<0||x>=tiles->getRows()||y>=tiles->getColumns()){
		return 0;
	}
	Tile * tile = tiles->getTile(cur.y,cur.x);
	if(y-1>=0){
	tile = tiles->getTile(y-1,x);
	if(!tile->collidable){
		neighbors[i] = Inds(x,y-1);
		i++;
	}}
	if(y+1<tiles->getRows()){
	tile = tiles->getTile(y+1,x);
	if(!tile->collidable){
		neighbors[i]= Inds(x,y+1);
		i++;
	}}
	if(x-1>=0){
	tile = tiles->getTile(y,x-1);
	if(!tile->collidable){
		neighbors[i]= Inds(x-1,y);
		i++;
	}}
	if(x+1<tiles->getRows()){
	tile = tiles->getTile(y,x+1);
	if(!tile->collidable){
		neighbors[i]= Inds(x+1,y);
		i++;
	}}
	
	if(y+1<tiles->getColumns()){
	if(x+1<tiles->getRows()){
	tile=tiles->getTile(y+1,x+1);
	if(!tile->collidable){
		tile = tiles->getTile(y,x+1);
		if(!tile->collidable){
			tile = tiles->getTile(y+1,x);
			if(!tile->collidable){
				neighbors[i]= Inds(x+1,y+1);
				i++;
			}
		}
	}}}
	if(x+1<tiles->getRows()){
	if(y-1>=0){
	tile=tiles->getTile(y-1,x+1);
	if(!tile->collidable){
		tile = tiles->getTile(y,x+1);
		if(!tile->collidable){
			tile = tiles->getTile(y-1,x);
			if(!tile->collidable){
				neighbors[i]= Inds(x+1,y-1);
				i++;
			}
		}
	}}}
	if(y+1<tiles->getColumns()){
	if(x-1>=0){
	tile=tiles->getTile(y+1,x-1);
	if(!tile->collidable){
		tile = tiles->getTile(y,x-1);
		if(!tile->collidable){
			tile = tiles->getTile(y+1,x);
			if(!tile->collidable){
				neighbors[i]= Inds(x-1,y+1);
				i++;
			}
		}
	}}}
	if(x-1>=0){
	if(y-1>=0){
	tile=tiles->getTile(y-1,x-1);
	if(!tile->collidable){
		tile = tiles->getTile(y,x-1);
		if(!tile->collidable){
			tile = tiles->getTile(y-1,x);
			if(!tile->collidable){
				neighbors[i]= Inds(x-1,y-1);
				i++;
			}
		}
	}}}
	return i;
}
bool contains(vector<Inds> * vec,Inds &ind){
	vector<Inds>::iterator it = vec->begin();
	for(;it!=vec->end();++it){
		if((*it)==ind){
			return true;
		}
	}
	return false;
}
multimap<int,Inds>::iterator contains(multimap<int,Inds> * m, Inds &ind){
	multimap<int,Inds>::iterator it = m->begin();
	for(;it!=m->end();++it){
		if((*it).second==ind){
			return it;
		}
	}
	return it;
}
map<Inds,Inds>::iterator contains(map<Inds,Inds> * m, Inds &ind){
	map<Inds,Inds>::iterator it = m->begin();
	for(;it!=m->end();++it){
		if((*it).second==ind){
			return it;
		}
	}
	return it;
}

int Bot::aveIters = 0;

void Bot::findPath(Game * game, Bot * bot, int toX, int toY){
	multimap<int,Inds> queue;
	vector<Inds> visited;
	map<Inds,int,cmp_tile> g_score;
	map<Inds,int,cmp_tile> h_score;
	map<Inds,int,cmp_tile> f_score;
	map<Inds,Inds> toKfromV;
	Inds cur;
	Inds dest;
	TiledLayer * tiles=(TiledLayer*) game->getWorld()->getLayers()->at(0);
	dest.x=tiles->calculateViewportStartColumn(toX);
	dest.y=tiles->calculateViewportStartRow(toY);
	if(tiles->getTile(dest.y,dest.x)->collidable){
		bot->pathLength=0;
		return;
	}
	AnimatedSprite * sprite = bot->getAnimatedSprite();
	PhysicalProperties * pp = sprite->getPhysicalProperties();
	cur.x=tiles->calculateViewportStartColumn(pp->getX());
	cur.y=tiles->calculateViewportStartRow(pp->getY());
//	cur.x--;
//	cur.y--;
	Inds src = cur;
	g_score.insert(pair<Inds,int>(cur,0));
	h_score.insert(pair<Inds,int>(cur,calc_H_Score(dest,cur)));
	f_score.insert(pair<Inds,int>(cur,h_score.find(cur)->second));
	queue.insert(pair<int,Inds>(f_score.find(cur)->second,cur));
	int iter = 1;
	while(!queue.empty() && iter<25){
		multimap<int,Inds>::iterator it = queue.begin();
		cur = (*it).second;
		if(cur==dest){break;}
		queue.erase(it);
		visited.push_back(cur);
		Inds neighbors[8];
		int numN = getNeighbors(cur,tiles,neighbors);
		for(int i = 0; i<numN; i++){
			Inds temp = neighbors[i];
			if(contains(&visited,temp)){
				continue;
			}
			int d = g_score.find(cur)->second + calc_H_Score(cur,temp);
			multimap<int,Inds>::iterator tit = contains(&queue,temp);
			if(tit==queue.end()){
				toKfromV.insert(pair<Inds,Inds>(temp,cur));
				g_score[temp] = d;
				h_score[temp] = calc_H_Score(dest,temp);
				f_score[temp] = g_score[temp]+h_score[temp];
				queue.insert(pair<int,Inds>(f_score[temp],temp));
			} else if(d<g_score[temp]){
				toKfromV.erase(temp);
				toKfromV.insert(pair<Inds,Inds>(temp,cur));
				g_score[temp] = d;
				h_score[temp] = calc_H_Score(dest,temp);
				f_score[temp] = g_score[temp]+h_score[temp];
				queue.erase(tit);
				queue.insert(pair<int,Inds>(f_score[temp],temp));
			}
		}
		iter++;
	}
	if(aveIters==0){
		aveIters=iter;
	} else {
		int prevAveIters = aveIters;
		aveIters=(aveIters+iter)/2;
		if(aveIters-prevAveIters>20){
			//do something for a break point...
			int dumb = 1+2;
		}
	}
	map<Inds,Inds>::iterator it = toKfromV.find(dest);
	vector<Inds> revPath;
	if(it!=toKfromV.end()){
		while(!(it->second==src)){
			revPath.push_back(it->first);
			it=toKfromV.find(it->second);
		}
		revPath.push_back(it->first);
		bot->pathLength=0;
		int i = revPath.size()-1;
		for(;i>=0;i--){
			Inds ind = revPath.at(i);
			bot->currentPath[bot->pathLength]->x=ind.x;
			bot->currentPath[bot->pathLength]->y=ind.y;
			bot->pathLength++;
		}
	}else{
		bot->pathLength=0;
	}
}
int tileWidth(Game * g){
	TiledLayer * tiles = (TiledLayer *)g->getWorld()->getLayers()->at(0);
	return tiles->getTileWidth();
}
int tileHeight(Game * g){
	TiledLayer * tiles = (TiledLayer *)g->getWorld()->getLayers()->at(0);
	return tiles->getTileHeight();
}
//DOES NOT WORK REPLACED BELOW FOR SIMPLER VERSION
void Bot::update(Game * g){	
	{//organizational block path finding
		TiledLayer * tiles = (TiledLayer *)g->getWorld()->getLayers()->at(0);
		PhysicalProperties * pp = animatedSprite->getPhysicalProperties();
		if(pathing == (path_calc % PATH)){
			PhysicalProperties * pl = g->getWorld()->getSpriteManager()->getPlayer()->getPhysicalProperties();
			findPath(g,this,pl->getX(),pl->getY());
			pathPoint = 0;
		}
		path_calc++;
		if(pathLength==0){
			pp->setVelocityX(0);
			pp->setVelocityY(0);
		} else {
			int px = pp->getX();
			int py = pp->getY();
			if(tiles->calculateViewportStartColumn(px)==currentPath[pathPoint]->x){
				if(tiles->calculateViewportStartRow(py)==currentPath[pathPoint]->y){
					pathPoint++;
					if(pathPoint == pathLength){
						//path_calc = PATH;
						pathPoint = 0;
					}
				}
			}
			int wid = tileWidth(g);
			int hig = tileHeight(g);
			int xNext = currentPath[pathPoint]->x * wid + 5;
			int yNext = currentPath[pathPoint]->y * hig + 5;
			int xVelD = px-xNext;
			int yVelD = py-yNext;
			if(xVelD!=0)
				xVelD = xVelD/abs(xVelD);
			if(yVelD!=0)
				yVelD = yVelD/abs(yVelD);
			pp->setVelocityX(-xVelD * RUNSPEED);
			pp->setVelocityY(-yVelD * RUNSPEED);
		}
	}//endd path finding rest is shooting
	AnimatedSprite * pl = g->getWorld()->getSpriteManager()->getPlayer();
	PhysicalProperties * playerPP = g->getWorld()->getSpriteManager()->getPlayer()->getPhysicalProperties();
	PhysicalProperties * pp = animatedSprite->getPhysicalProperties();
	BoundingVolume * p = pl->getBoundingVolume();
	BoundingVolume * bv = animatedSprite->getBoundingVolume();
	float Y = (((float)playerPP->getY())+p->getHeight()/2.0)-((float)pp->getY()+bv->getHeight()/2.0);
	float X = (((float)playerPP->getX())+p->getWidth()/2.0)-((float)pp->getX()+bv->getWidth()/2.0);
	float angle=atan2f(Y,X);
	float mag = sqrt(X*X+Y*Y);
	float unitX = X/mag;
	float unitY = Y/mag;
	animatedSprite->fire(g,angle,right,unitX,unitY);
}
/*void Bot::update(Game * g){
	AnimatedSprite * pl = g->getWorld()->getSpriteManager()->getPlayer();
	PhysicalProperties * playerPP = g->getWorld()->getSpriteManager()->getPlayer()->getPhysicalProperties();
	PhysicalProperties * pp = animatedSprite->getPhysicalProperties();
	BoundingVolume * bv = animatedSprite->getBoundingVolume();
	if(animatedSprite->hp<=0)
	{
		if(dead==false) {
			((GameMusic*)g->getGameAudio())->playEffect("dead");
			dead=true;
		}
		animatedSprite->setCurrentState(2);
		return;
	}
	if(!g->getWorld()->getViewport()->areWorldCoordinatesInViewport(bv->getX(),bv->getY(),bv->getWidth(),bv->getHeight()))
	{
		return;
	}
	bool right;
	if(playerPP->getX()-pp->getX()<0) 
	{
		right=false;
		animatedSprite->setCurrentState(1);
	} 
	else 
	{
		right=true;
		animatedSprite->setCurrentState(0);
	}
	BoundingVolume * p = pl->getBoundingVolume();
	float Y = (((float)playerPP->getY())+p->getHeight()/2.0)-((float)pp->getY()+bv->getHeight()/2.0);
	float X = (((float)playerPP->getX())+p->getWidth()/2.0)-((float)pp->getX()+bv->getWidth()/2.0);
	float angle=atan2f(Y,X);
	float mag = sqrt(X*X+Y*Y);
	float unitX = X/mag;
	float unitY = Y/mag;
	animatedSprite->fire(g,angle,right,unitX,unitY);
}
*/