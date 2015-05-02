
#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "resource.h"
#include "stdafx.h"
#include <vector>
#include "AIPlayer.h"
#include "AnimatedSprite.h"
#include "TiledLayer.h"
#define HORIZ_WIEGHT 10
#define VERT_WIEGHT 10
#define DIAG_WIEGHT 14

struct Inds{
	int x;
	int	y;
	Inds(int nx,int ny){
		x=nx;
		y=ny;
	}
	Inds(){x=0;y=0;}
};

struct cmp_tile {
	bool operator()(Inds const  a, Inds  const b) {
		return (a.x*a.x + a.y*a.y)<(b.x*b.x+b.y*b.y);
	}
};

class Bot
{
protected:
	AIPlayer *animatedSprite;
	Inds * currentPath[100];
	float density;
	int pathLength;
	int PATH;
	int pathing;
	int path_calc;
	int pathPoint;
	bool dead;
	static int aveIters;

public:
	// INLINED METHODS
	AnimatedSprite* getAnimatedSprite() { return animatedSprite; }
	float getDensity() { return density; }
	void setDensity(float initDensity) { density = initDensity; }
	void setSprite(AIPlayer * sprite){ animatedSprite=sprite;}
	bool isDead() {return dead;}
	Bot(){
		dead=false;
		PATH=30;
		for(int i = 0; i<50; i++){
			currentPath[i] = new Inds;
		}
		pathLength = 0;
		srand(12);
		pathing = rand() % PATH;
		path_calc = rand() % PATH;
		pathPoint = 0;
	};
	~Bot(){};
	virtual void update(Game *game);
	static void findPath(Game * game, Bot * bot, int toX, int toY);
};