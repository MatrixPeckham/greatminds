#pragma once 
#include "CollidableObject.h"
#include "Game.h"
#include "AnimatedSprite.h"
#include "Explosion.h"
#include <vector>

#include "Collisions.h"

#define MAX_COLLISIONS 100
class HitTest {
public:
	HitTest();
	~HitTest();
	bool hitTest(CollidableObject* obj1,CollidableObject* obj2 );
	void sort();
	void clear();
	void reset();
	void handle(int i);
	void buildHeap();
	collision* get(int i);
	bool checkIfColliding(CollidableObject* obj1, CollidableObject* obj2);
	bool checkXCollide(CollidableObject* obj1, CollidableObject* obj2);
	bool checkYCollide(CollidableObject* obj1, CollidableObject* obj2);
	void checkAllTileCollisions(Game* g, AnimatedSprite* obj);
	void checkAllSpriteCollisions(Game* g, AnimatedSprite* obj, AnimatedSprite* obj2);
	void handleCollisionX(int i);
	void handleCollisionY(int i);
	int size() {return ptr;}
	void setPtr(int i) {ptr=i;}
	void invert();
	float calculateHitTime(CollidableObject* obj1, CollidableObject* obj2);
	collision** collisions;
private:
	//collision** collisions;
	int ptr;
	bool applyG;
	void heapify(int i, int a);
	
	float getFirstTimeX(CollidableObject* obj1,CollidableObject* obj2);
	float getLastTimeX(CollidableObject* obj1,CollidableObject* obj2);
	float getFirstTimeY(CollidableObject* obj1,CollidableObject* obj2);
	float getLastTimeY(CollidableObject* obj1,CollidableObject* obj2);
};