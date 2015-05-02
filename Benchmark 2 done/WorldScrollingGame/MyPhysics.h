#pragma once
#include "HitTest.h"
#include "GamePhysics.h"
#include "Game.h"
#include "Explosion.h"
#define BASEID 1
class MyPhysics : GamePhysics {
private: 
	HitTest* h;
	Explosion* exp;
	bool needExplosion;
	//Temporary for testing hits to base and message
	CollidableObject* tmp;
	PhysicalProperties* pp;
	BoundingVolume* bv;
public:
	MyPhysics();
	void messageHit(Game* g);
	void update(Game* g);
	void baseHit(Game* g);
	void handleCollide(Game* g);
	void handleBulletCollide();
};