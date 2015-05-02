#include "AnimatedSprite.h"
#include "AnimatedSpriteType.h"
#include "BoundingVolume.h"
#include "CollidableObject.h"
#include "Game.h"
#include "GameAI.h"
#include "Projectile.h"
#include "Player.h"
#include "GameDataLoader.h"
#include "GameGraphics.h"
#include "DeterministicBot.h"
#include "GameWorld.h"
#include "PhysicalProperties.h"
#include "TiledLayer.h"
#include "SparseLayer.h"
#include "SpriteManager.h"
#include "Viewport.h"
#include "time.h"
#include "World.h"
#include "Constants.h"
#include "AIPlayer.h"
#include "GameMusic.h"
#include <fstream>
#include "Explosion.h"

extern int GAME_MODE;
using namespace std;
void loadWeapons(Game*g);
void allocateWeaponsSprites(Game* g, AnimatedSpriteType* t);
void loadAIPlayers(Game* g, wifstream &file);
void loadAIPlayer(Game* game);
void loadLevel(Game* g, int i);
void loadSpriteType(Game* g, wchar_t* i);
void loadSprite(Game *game, int type, int sindex, int x, int y);
void loadExplosion(Game *g);
const wchar_t* fromString(string s);

/*
	loadLevelExample - This method loads the current
	level with data. It illustrates all the data
	we would need to load ourselves. You should
	load your data by reading it from a file.
*/
void loadLevelExample(Game *game)
{
	// FIRST SETUP THE GAME WORLD DIMENSIONS
	GameWorld *world = game->getWorld();
	GameGraphics *graphics = game->getGraphics();
	TextureManager *worldTextureManager = graphics->getWorldTextureManager();
	Viewport *viewport = world->getViewport();
	viewport->setViewportWidth(1024);
	viewport->setViewportHeight(704);
	viewport->setViewportOffsetX(0);
	viewport->setViewportOffsetY(64);
	world->setWorldWidth(1920);
	world->setWorldHeight(1280);

	// NOW LOAD A TILED BACKGROUND
	TiledLayer *tiledLayer = new TiledLayer(30, 20, 64, 64, 0, true, viewport, 1920, 1280);

	wchar_t *fileName = constructEmptyWCHAR_TArray(L"textures/world/tiles/grass.bmp");
	int grassID = worldTextureManager->loadTexture(fileName);

	fileName = constructEmptyWCHAR_TArray(L"textures/world/tiles/wall.bmp");

	int wallID = worldTextureManager->loadTexture(fileName);
	srand(time(NULL));
	for (int i = 0; i < 600; i++)
	{
		bool isCollidable = false;;
		int tileIDToUse = grassID;
		int randomInt = rand() % 100;
		if (randomInt >= 50)
			isCollidable = true;

		randomInt = rand() % 100;
		if (randomInt >= 80)
			tileIDToUse = wallID;

		Tile *tileToAdd = new Tile();
		tileToAdd->collidable = isCollidable;
		tileToAdd->textureID = tileIDToUse;
		tiledLayer->addTile(tileToAdd);
	}
	world->addLayer(tiledLayer);

	// AND NOW LET'S MAKE A MAIN CHARACTER SPRITE
	AnimatedSpriteType *ast = new AnimatedSpriteType();
	fileName = constructEmptyWCHAR_TArray(L"textures/world/sprites/tros/tros_3_0.bmp");
	int spriteImageID1 = worldTextureManager->loadTexture(fileName);
	ast->addImageID(spriteImageID1);
	fileName = constructEmptyWCHAR_TArray(L"textures/world/sprites/tros/tros_3_1.bmp");
	int spriteImageID2 = worldTextureManager->loadTexture(fileName);
	ast->addImageID(spriteImageID2);
	fileName = constructEmptyWCHAR_TArray(L"textures/world/sprites/tros/tros_3_2.bmp");
	int spriteImageID3 = worldTextureManager->loadTexture(fileName);
	ast->addImageID(spriteImageID3);

	// CHANGE THE IMAGE ONCE EVERY 6 FRAMES
	ast->setAnimationSpeed(6);

	// SIZE OF SPRITE IMAGES
	ast->setTextureSize(64, 96);

	// NOW LET'S ADD AN ANIMATION STATE
	// FIRST THE NAME
	wchar_t *animStateName = constructEmptyWCHAR_TArray(L"STANDING");
	ast->addAnimationState(animStateName);
	vector<int> *standingSequence = new vector<int>();

	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID2);
	ast->addAnimationFrame(0, spriteImageID3);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID2);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID3);
	ast->addAnimationFrame(0, spriteImageID3);
	ast->addAnimationFrame(0, spriteImageID3);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID2);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID3);
	ast->addAnimationFrame(0, spriteImageID1);
	ast->addAnimationFrame(0, spriteImageID1);

	SpriteManager *spriteManager = world->getSpriteManager();
	spriteManager->addSpriteType(ast);

	AnimatedSprite *player = new AnimatedSprite();
	player->setSpriteType(ast);
	PhysicalProperties *playerProps = new PhysicalProperties();
	player->setPhysicalProperties(playerProps);
	playerProps->setX(10);
	playerProps->setY(10);
	playerProps->setAccelerationX(0);
	playerProps->setAccelerationY(0);
	// WE WILL SET LOTS OF OTHER PROPERTIES ONCE
	// WE START DOING COLLISIONS AND PHYSICS

	player->setAlpha(255);
	player->setCurrentState(STANDING_FACING_RIGHT);

	spriteManager->setPlayer(player);
}

/*
	loadWorld - This method should load the data
	for the GameWorld's current level. The GameWorld
	stores a vector of all level file names. Such
	a file would describe how a level would be
	constructed.
*/
void GameDataLoader::loadWorld(Game *game)	
{
	// NOTE:	I AM DEMONSTRATING HOW TO LOAD A LEVEL
	//			PROGRAMICALLY. YOU SHOULD DO THIS
	//			USING CSV FILES.
	loadLevel(game, game->getWorld()->getCurrentLevel());
	game->getAI()->init(game);
}

/*
	loadGUI - One could use this method to build
	the GUI based on the contents of a GUI file.
	That way we could change the GUI and add to
	it without having to rebuild the project.
*/
void GameDataLoader::loadGUI(Game *game)
{
	
	
}
void loadLevel(Game* game, int i) {
	GameWorld* gw=game->getWorld();
	wchar_t *plname = NULL;


	wifstream file;
	file.open(gw->getLevelFileName(i));
	switch(i) {
		case 1:
		((GameMusic*)game->getGameAudio())->playNewSong("lv1");
		plname = constructEmptyWCHAR_TArray(L"./levels/level1.lvpl");
		break;
		case 2:
		((GameMusic*)game->getGameAudio())->playNewSong("lv2");
		plname = constructEmptyWCHAR_TArray(L"./levels/level3.lvpl");
		break;
		case 3:
		((GameMusic*)game->getGameAudio())->playNewSong("lv3");
		plname = constructEmptyWCHAR_TArray(L"./levels/level2.lvpl");
		break;
		default:
		((GameMusic*)game->getGameAudio())->playNewSong("lv1");
		plname = constructEmptyWCHAR_TArray(L"./levels/level4.lvpl");
	}
	int width, height, tileW, tileH;
	file>>height;
	file>>width;
	file>>tileW;
	file>>tileH;


	GameGraphics *graphics = game->getGraphics();
	TextureManager *textures = graphics->getWorldTextureManager();
	Viewport *viewport = gw->getViewport();
	viewport->setViewportX(0);
	viewport->setViewportY(0);
	viewport->setViewportWidth(1024);
	viewport->setViewportHeight(704);
	viewport->setViewportOffsetX(0);
	viewport->setViewportOffsetY(75);
	gw->setWorldWidth(width*tileW);
	gw->setWorldHeight(height*tileH);

	// NOW LOAD A TILED BACKGROUND
	TiledLayer *tl = new TiledLayer(width, height, tileW, tileH, 0, true, viewport, width*tileW, height*tileH);
	SparseLayer *sp =new SparseLayer();
	wchar_t *line=new wchar_t[256];
	file.getline(line,255);
	file.getline(line,255);
	int k=0;//number of tiled images
	while(wcscmp(line, L"\\0")!=0 && !file.eof()){
		if(textures->loadTexture(constructEmptyWCHAR_TArray(line))!=-1) {
			k++;
		}
		file.getline(line,255);
		
	}
	if(!file.eof()) {
		//load sparseimages
	file.getline(line,255);
	while(wcscmp(line, L"\\0")!=0 && !file.eof()){
		textures->loadTexture(constructEmptyWCHAR_TArray(line));
		file.getline(line,255);
		
	}
	}

	for(int i=0; i<width; i++) {
		for(int j=0; j<height; j++) {
			int id;
			file>>id;
			Tile* tile=new Tile();
			tile->textureID=id;
			tile->collidable=false;
			if(id==2 || id==3 || id==4){
				tile->collidable=true;
			} else if(id==0){
			}
			tl->addTile(tile);
		}
	}
	//two null lines
	file.getline(line,255);
	file.getline(line,255);
	file.getline(line,255);
	while(!file.eof()) {
		int id, x,y;
		file>>id;
		if(GAME_MODE==0 || id!=0) {
		file>>x;
		file>>y;
		id=id+k;
		OverlayImage* ol=new OverlayImage();
		ol->alpha=255;
		ol->imageID=id;
		ol->x=x*tileW;
		ol->y=y*tileH;
		ol->width=tileW;
		ol->height=tileH;
		sp->addTile(ol);
		sp->setCollidableTiles(true);
		}
		file.getline(line,255);
	}
		gw->addLayer(tl);
		gw->addLayer(sp);
		//Load Animations from file
		wifstream animfile;
		animfile.open("./animations/master.txt");
		wchar_t* animfileName;
		wchar_t *animation=new wchar_t[256];
		animfile.getline(animation,255);
		while(wcscmp(animation, L"\\0")!=0 && !animfile.eof()) {
			loadSpriteType(game, animation);
			animfile.getline(animation,255);
		}
		wifstream plfile;
		plfile.open(plname);
		while(!animfile.eof()) {
			int sindex, type, x, y;
			animfile>>sindex;
			animfile>>type;
			plfile>>x;
			plfile>>y;
			x*=tl->getTileWidth();
			y*=tl->getTileHeight();
			x++;
			y++;
			viewport->moveViewport(x-viewport->getViewportWidth()/2,y-viewport->getViewportHeight()/2,gw->getWorldWidth(),gw->getWorldHeight());
			loadSprite(game, type, sindex, x, y);
			animfile.getline(animation,255);
		}
		plfile.close();
		animfile.close();
		loadAIPlayer(game);
		loadExplosion(game);
		loadWeapons(game);
		delete animation;
		file.close();
}

void loadExplosion(Game* g)
{
	TextureManager* t=g->getGraphics()->getWorldTextureManager();

	AnimatedSpriteType* exp = new AnimatedSpriteType();
	exp->setSpriteTypeID(4444);

	wchar_t* animfileName = constructEmptyWCHAR_TArray(L"./images/expl1.png");
	int explImageID1 = t->loadTexture(animfileName);
	exp->addImageID(explImageID1);
	animfileName = constructEmptyWCHAR_TArray(L"./images/expl2.png");
	int explImageID2 = t->loadTexture(animfileName);
	exp->addImageID(explImageID2);
	animfileName = constructEmptyWCHAR_TArray(L"./images/expl3.png");
	int explImageID3 = t->loadTexture(animfileName);
	exp->addImageID(explImageID3);
	animfileName = constructEmptyWCHAR_TArray(L"./images/expl4.png");
	int explImageID4 = t->loadTexture(animfileName);
	exp->addImageID(explImageID4);
	animfileName = constructEmptyWCHAR_TArray(L"./images/expl5.png");
	int explImageID5 = t->loadTexture(animfileName);
	exp->addImageID(explImageID5);
	animfileName = constructEmptyWCHAR_TArray(L"./images/expl6.png");
	int explImageID6 = t->loadTexture(animfileName);
	exp->addImageID(explImageID6);
	animfileName = constructEmptyWCHAR_TArray(L"./images/expl7.png");
	int explImageID7 = t->loadTexture(animfileName);
	exp->addImageID(explImageID7);

	exp->setAnimationSpeed(3);
	exp->setTextureSize(32,32);

	// NOW LET'S ADD AN ANIMATION STATE
	// FIRST THE NAME
	wchar_t* animStateName = constructEmptyWCHAR_TArray(L"EXPLOSION");
	exp->addAnimationState(animStateName);
	vector<int> *explosionSequence = new vector<int>();

	exp->addAnimationFrame(0, explImageID1);
	exp->addAnimationFrame(0, explImageID2);
	exp->addAnimationFrame(0, explImageID3);
	exp->addAnimationFrame(0, explImageID4);
	exp->addAnimationFrame(0, explImageID5);
	exp->addAnimationFrame(0, explImageID6);
	exp->addAnimationFrame(0, explImageID7);

	exp->addAnimationSequence(explosionSequence);

	animStateName = constructEmptyWCHAR_TArray(L"invisible");
	exp->addAnimationState(animStateName);
	vector<int> *invisibleSequence = new vector<int>();

	exp->addAnimationFrame(1, explImageID7);

	g->getWorld()->getSpriteManager()->addSpriteType(exp);
	for(int i=0; i<20;i++) {
		AnimatedSprite *explosion1 = new AnimatedSprite();
		explosion1->setSpriteType(exp);
		PhysicalProperties *expProp = new PhysicalProperties();
		explosion1->setPhysicalProperties(expProp);
		expProp->setX(64);
		expProp->setY(64);
		expProp->setVelocityX(0);
		expProp->setVelocityY(0);
		expProp->setAccelerationX(0);
		expProp->setAccelerationY(0);
		explosion1->setAlpha(255);
		explosion1->setCurrentState(1);
		explosion1->changeFrame();
		explosion1->setCurrentlyCollidable(false);

		BoundingVolume *bv = new BoundingVolume();
		bv->setX(expProp->getX());
		bv->setY(expProp->getY());
		bv->setHeight(32);
		bv->setWidth(32);

		explosion1->setBoundingVolume(bv);
		((Explosion*)explosion1)->setDead(true);
		g->getWorld()->getSpriteManager()->addSprite(explosion1);

	}
	
}
void loadAIPlayer(Game * game){
	TiledLayer * tiles = (TiledLayer * ) game->getWorld()->getLayers()->at(0);
	AnimatedSpriteType * aiT = new AnimatedSpriteType();

	aiT->addAnimationState(constructEmptyWCHAR_TArray(L"state"));
	int id = game->getGraphics()->getWorldTextureManager()->loadTexture(constructEmptyWCHAR_TArray(L"images/Enemies/AI_DUMMY.png"));
	
	aiT->addAnimationFrame(0,id);
	aiT->addAnimationFrame(0,id);
	aiT->setSpriteTypeID(99);
	aiT->setTextureSize(64,64);
	aiT->setAnimationSpeed(1);

	wchar_t* animfileName = constructEmptyWCHAR_TArray(L"./images/Enemies/AI_dead.png");
	int AI_dead_ID = game->getGraphics()->getWorldTextureManager()->loadTexture(animfileName);
	aiT->addImageID(AI_dead_ID);

	animfileName = constructEmptyWCHAR_TArray(L"./images/Enemies/AI_left.png");
	int AI_left_ID = game->getGraphics()->getWorldTextureManager()->loadTexture(animfileName);
	aiT->addImageID(AI_left_ID);

	animfileName = constructEmptyWCHAR_TArray(L"./images/Enemies/aiHitRight.png");
	int AI_hitRight_ID = game->getGraphics()->getWorldTextureManager()->loadTexture(animfileName);
	aiT->addImageID(AI_left_ID);

	animfileName = constructEmptyWCHAR_TArray(L"./images/Enemies/aiHitLeft.png");
	int AI_hitLeft_ID = game->getGraphics()->getWorldTextureManager()->loadTexture(animfileName);
	aiT->addImageID(AI_left_ID);

	wchar_t* animStateName = constructEmptyWCHAR_TArray(L"LEFT");
	aiT->addAnimationState(animStateName);
	vector<int> *leftSequence = new vector<int>();

	aiT->addAnimationFrame(1, AI_left_ID);
	aiT->addAnimationSequence(leftSequence);

	animStateName = constructEmptyWCHAR_TArray(L"DEAD");
	aiT->addAnimationState(animStateName);
	vector<int> *deadSequence = new vector<int>();

	aiT->addAnimationFrame(2, AI_dead_ID);
	aiT->addAnimationSequence(deadSequence);

	animStateName = constructEmptyWCHAR_TArray(L"HITRIGHT");
	aiT->addAnimationState(animStateName);
	vector<int> *hitRight = new vector<int>();

	aiT->addAnimationFrame(3, AI_hitRight_ID);
	aiT->addAnimationSequence(hitRight);

	animStateName = constructEmptyWCHAR_TArray(L"HITLEFT");
	aiT->addAnimationState(animStateName);
	vector<int> *hitLeft = new vector<int>();

	aiT->addAnimationFrame(4, AI_hitLeft_ID);
	aiT->addAnimationSequence(hitLeft);

	SpriteManager * sm = game->getWorld()->getSpriteManager();
	sm->addSpriteType(aiT);
	int width = tiles->getColumns();
	int height = tiles->getRows();
	for(int i=0; i<width; i++) {
		for(int j=0; j<height; j++) {
			Tile* tile= tiles->getTile(j,i);
			int id = tile->textureID;
			if(id==0){
				Bot * b = new Bot();
				AIPlayer * sp = new AIPlayer(game);
				PhysicalProperties * pp = new PhysicalProperties();
				int x = i * tiles->getTileWidth();
				int y = j * tiles->getTileHeight();
				pp->setX(x);
				pp->setY(y);
				pp->setVelocityX(0);
				pp->setVelocityY(0);
				pp->setAccelerationX(0);
				pp->setAccelerationY(0);
				BoundingVolume * bv = new BoundingVolume();
				bv->setX(x);
				bv->setY(y);
				bv->setWidth(64);
				bv->setHeight(64);
				sp->setPhysicalProperties(pp);
				sp->setBoundingVolume(bv);
				sp->setSpriteType(aiT);
				sp->setAlpha(255);
				sp->changeFrame();
				sp->setCurrentState(0);
				int l = 20;
				int h = 60;
				int a = 15;
				sp->setLowFireRate(l);
				sp->setHighFireRate(h);
				sp->setAccuracy(a);
				b->setSprite(sp);
				sm->addSprite(sp);
				game->getAI()->addBot(b);
			}
		}
	}

}
void loadAIPlayers(Game* game, wifstream &file) {
	TiledLayer * tiles =(TiledLayer *) game->getWorld()->getLayers()->at(0);
	wchar_t *line=new wchar_t[256];
		if(!file.eof()) {
		//load AIs
	file.getline(line,255);
	AnimatedSpriteType * aiT = new AnimatedSpriteType();
	aiT->addAnimationState(constructEmptyWCHAR_TArray(L"state"));
	int id = game->getGraphics()->getWorldTextureManager()->loadTexture(constructEmptyWCHAR_TArray(L"images/Enemies/AI_DUMMY.png"));
	aiT->addAnimationFrame(0,id);
	aiT->addAnimationFrame(0,id);
	aiT->setSpriteTypeID(99);
	aiT->setTextureSize(64,64);
	aiT->setAnimationSpeed(1);
	SpriteManager * sm = game->getWorld()->getSpriteManager();
	sm->addSpriteType(aiT);
	while(wcscmp(line, L"\\0")!=0 && !file.eof()){
	

	Bot * b = new Bot();
	AIPlayer * sp = new AIPlayer(game);
	PhysicalProperties * pp = new PhysicalProperties();
	int x;
	file>>x;
	pp->setX(x);
	int y;
	file>>y;
	pp->setY(600);
	pp->setVelocityX(0);
	pp->setVelocityY(0);
	pp->setAccelerationX(0);
	pp->setAccelerationY(0);
	BoundingVolume * bv = new BoundingVolume();
	bv->setX(x);
	bv->setY(y);
	bv->setWidth(64);
	bv->setHeight(64);
	sp->setPhysicalProperties(pp);
	sp->setBoundingVolume(bv);
	sp->setCurrentState(0);
	sp->setSpriteType(aiT);
	sp->setAlpha(255);
	sp->setCurrentState(0);
	int l;
	file>>l;
	int h;
	file>>h;
	int a;
	file>>a;
	sp->setLowFireRate(l);
	sp->setHighFireRate(h);
	sp->setAccuracy(a);
	b->setSprite(sp);
	sm->addSprite(sp);
	game->getAI()->addBot(b);
	file.getline(line,255);
		
	}
	}
	delete line;
}

void loadSpriteType(Game* game, wchar_t* file) {
	GameWorld* gw=game->getWorld();
	GameGraphics *graphics = game->getGraphics();
	TextureManager *textures = graphics->getWorldTextureManager();
	wifstream animfile;
		animfile.open(constructEmptyWCHAR_TArray(file));
		wchar_t* animfileName;
		wchar_t *animStateName;
		wchar_t * line=new wchar_t[256];
		int stateID=0;
		AnimatedSpriteType *ast = new AnimatedSpriteType();
		int speed, w,h;
			animfile>>speed;
			animfile>>w;
			animfile>>h;
			ast->setAnimationSpeed(speed);
			ast->setTextureSize(w, h);
			animfile.getline(line, 255);
		while(!animfile.eof()) {
			animfile.getline(line, 255);
			if(wcscmp(line,L"\\0")==0) {
				break;
			}
			animStateName = constructEmptyWCHAR_TArray(line);
			ast->addAnimationState(animStateName);
			animfile.getline(line, 255);
			int spriteImageID;
			int base=-1;
			while(wcscmp(line,L"\\0")!=0 && !animfile.eof()) {
				animfileName= constructEmptyWCHAR_TArray(line);
				spriteImageID = textures->loadTexture(animfileName);
				if(base==-1) {
					base=spriteImageID;
				}
				ast->addImageID(spriteImageID);
				animfile.getline(line, 255);
			}
			
			int frame;
			animfile>>frame;
			while(frame!=-1) {
				ast->addAnimationFrame(stateID, frame+base);
				animfile>>frame;
			}
			stateID++;
			animfile.getline(line,255);
		}
	

	SpriteManager *spriteManager = gw->getSpriteManager();
	spriteManager->addSpriteType(ast);
	delete line;
}
void loadSprite(Game *game, int type, int sindex, int x, int y) {
	GameWorld* gw=game->getWorld();
	GameGraphics *graphics = game->getGraphics();
	TextureManager *textures = graphics->getWorldTextureManager();
	SpriteManager *spriteManager = gw->getSpriteManager();

	Player *player = new Player(game);
	player->setSpriteType(spriteManager->getSpriteType(sindex));
	
	PhysicalProperties *playerProps = new PhysicalProperties();
	
	playerProps->setX(x);
	playerProps->setY(y);
	playerProps->setAccelerationX(0);
	//playerProps->setAccelerationY(GRAVITY);
	BoundingVolume* v=new BoundingVolume();
	
	v->setWidth(48);//player->getSpriteType()->getTextureWidth()
	v->setHeight(48);
	v->setX(playerProps->getX());
	v->setY(playerProps->getY());
	player->setPhysicalProperties(playerProps);
		player->setBoundingVolume(v);
	// WE WILL SET LOTS OF OTHER PROPERTIES ONCE
	// WE START DOING COLLISIONS AND PHYSICS

	player->setAlpha(255);
	player->setCurrentState(STANDING_FACING_RIGHT);
	player->setCurrentlyCollidable(true);
	player->setTakeFlag(false);
	player->changeFrame();
	//if(type==0) {
	spriteManager->setPlayer(player);
	//} else {
		//DeterministicBot* b=new DeterministicBot();
		//b->setPlayer(0,player, 1);
	//	spriteManager->addSprite(player);
		
	//}
	
}

void loadWeapons(Game* g) {
	AnimatedSpriteType* ast=new AnimatedSpriteType();
	ast->addAnimationState(L"armed");
	TextureManager* t=g->getGraphics()->getWorldTextureManager();
	wchar_t* animfileName= constructEmptyWCHAR_TArray(L"./images/bomb.png");
	int spriteImageID = t->loadTexture(animfileName);
	ast->addImageID(spriteImageID);
	ast->setSpriteTypeID(100);
	ast->addAnimationFrame(0,8);
	ast->setAnimationSpeed(1);
	ast->setTextureSize(32,32);
	g->getWorld()->getSpriteManager()->addSpriteType(ast);

	/*TextureManager* t=g->getGraphics()->getWorldTextureManager();

	AnimatedSpriteType* ast = new AnimatedSpriteType();

	wchar_t* animfileName= constructEmptyWCHAR_TArray(L"./images/bomb.png");
	int spriteImageID = t->loadTexture(animfileName);
	ast->addImageID(spriteImageID);

	ast->setAnimationSpeed(1);
	ast->setTextureSize(32,32);

	wchar_t *animStateName = constructEmptyWCHAR_TArray(L"armed");
	ast->addAnimationState(animStateName);
	vector<int> *armedSequence = new vector<int>();

	ast->addAnimationFrame(0,spriteImageID);

	ast->addAnimationSequence(armedSequence);*/

	
	AnimatedSpriteType* ast2=new AnimatedSpriteType();
	ast2->addAnimationState(L"bullet");
	animfileName= constructEmptyWCHAR_TArray(L"./images/bullet.png");
	spriteImageID = t->loadTexture(animfileName);
	ast2->setSpriteTypeID(101);
	ast2->addImageID(spriteImageID);
	ast2->addAnimationFrame(0,spriteImageID);
	ast2->setAnimationSpeed(1);
	ast2->setTextureSize(4,2);
	g->getWorld()->getSpriteManager()->addSpriteType(ast2);
	//LoadBullet
	SpriteManager* sm=g->getWorld()->getSpriteManager();
	sm->setBulletInd(sm->getTypes()-1);
	allocateWeaponsSprites(g, ast);
	//Load Bomb
	
}
void allocateWeaponsSprites(Game* g, AnimatedSpriteType* t) {
	SpriteManager* sm=g->getWorld()->getSpriteManager();
	sm->markWeaponsStart(sm->getNumberOfSprites());
	for(int i=0; i<MAX_WEAPONS;i++) {
		Projectile* tmp=new Projectile();
		PhysicalProperties* pp=new PhysicalProperties();
		BoundingVolume* bv=new BoundingVolume();
		pp->setVelocityX(0);
		pp->setVelocityY(0);
		pp->setX(-6000);
		pp->setY(6000);
		pp->setAccelerationX(0);
		pp->setAccelerationY(0);
		tmp->setBoundingVolume(bv);
		tmp->setPhysicalProperties(pp);
		tmp->setSpriteType(t);
		tmp->setAlpha(255);
		tmp->setWeapon(0,0,0);
		tmp->setCurrentState(0);
		tmp->changeFrame();
		sm->addSprite(tmp);
		
	}
}

