/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WorldRenderingGame.cpp

	This is a test game application, a game that
	demonstrates use of	the game engine simply
	for rendering images and text as well as for reacting to
	key presses and button cicks. Students will extend
	this for their projects, gradually adding other components,
	like additional gui controls, tiling, sprites, collision 
	detection, etc.
 */

#include "stdafx.h"
#include "MoreGameStates.h"

// GAME OBJECT INCLUDES
#include "Cursor.h"
#include "ScreenGUI.h"
#include "Game.h"
#include "GameGraphics.h"
#include "GameGUI.h"
#include "GameInput.h"
#include "GameOS.h"
#include "GameText.h"
#include "MyPhysics.h"
#include "GameMusic.h"

// WINDOWS PLATFORM INCLUDES
#include "WindowsGameOS.h"
#include "WindowsGameInput.h"
#include "WindowsGameTimer.h"
#include "WindowsGameRenderThread.h"

// DIRECTX INCLUDES
#include "DirectXGraphics.h"
#include "DirectXTextureManager.h"

//Handlers 
#include "KeyHandler.h"
#include "ButtonHandler.h"
#include "Text.h"
#include "MyTextClass.h"
#include "MouseInput.h"

extern int GAME_MODE =1;

// METHODS OTHER THAN WinMain DEFINED BELOW
wchar_t*	constructEmptyWCHAR_TArray(LPCWSTR initChars);
void		allocateWeaponSprites(Game* g);
void		createGui(Game *game);
void		pauseGui(Game *game);
void		confirmGui(Game *game, wchar_t* conf);
void		aboutGui(Game *game,wchar_t* a);
void		gameOverGui(Game *game);
void		gameSelectGui(Game *game);
void		gameWinGui(Game *game);

/*
	WinMain - This is the application's starting point. In this
	method we will construct a Game object, then construct all the
	custom data for our game, and then initialize the Game with
	our custom data. We'll then start the game loop.
*/
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	Game* mygame=new Game();
	
	mygame->setPhysics((GamePhysics*)(new MyPhysics()));
	GameMusic* music=new GameMusic(L"./Music/Win/Background.xwb", L"./Music/Win/BGBank.xsb",L"./Music/Win/Effects.xwb",L"./Music/Win/EffectsBank.xsb", "menu", mygame);
	mygame->setGameAudio((GameAudio*)music);
	//the game init function takes a path, graphics, game os, game input, game timer
	//1.) path
	wchar_t* path=constructEmptyWCHAR_TArray(L"/data/");
	//3.) Os-because we need it to initialize graphics 
	wchar_t* title=constructEmptyWCHAR_TArray(L"Great Minds");
	WindowsGameOS* windows=new WindowsGameOS(hInstance,nCmdShow,false,title,mygame);
	//2.) Graphics 
	DirectXGraphics* graphics=new DirectXGraphics(mygame);
	graphics->init();
	graphics->initGraphics(windows->getWindowHandle(),false);
	//4.) Game input
	WindowsGameInput* input=(WindowsGameInput*) (new MouseInput());
	//5.) Game Timet
	WindowsGameTimer* timer=new WindowsGameTimer();
	//6.) Game Render Thread
	WindowsGameRenderThread * renderThread = new WindowsGameRenderThread();

	//create game
	mygame->init(path,graphics,windows,input,timer,renderThread);
	//GUI in order of game states
	//pauseGui(mygame);
	createGui(mygame);
	pauseGui(mygame);
	confirmGui(mygame,EXIT_DIALOG);
	aboutGui(mygame, ABOUTMENU);
	aboutGui(mygame, HELPMENU);
	aboutGui(mygame, CONTROLS);
	confirmGui(mygame,NEW_DIALOG);
	gameOverGui(mygame);
	gameSelectGui(mygame);
	gameWinGui(mygame);

	KeyHandler* key=new KeyHandler();
	mygame->getInput()->registerKeyHandler(key);

	ButtonHandler* button=new ButtonHandler();
	mygame->getGUI()->registerButtonEventHandler((ButtonEventHandler*)button);

	//((WindowsGameTimer*)mygame->getTimer())->setTargetFPS(30);
	WRTextGenerator *textGenerator = new WRTextGenerator();
	textGenerator->initText(mygame);
		mygame->getText()->setTextGenerator((TextGenerator*)textGenerator);
	
	
	GameWorld *world = mygame->getWorld();
	//wchar_t *levelFile = constructEmptyWCHAR_TArray(L"./levels/level1.lv");
	//temp
	wchar_t *levelFile = constructEmptyWCHAR_TArray(L"./levels/level1.lv");
	world->addLevelFileName(levelFile);
	levelFile = constructEmptyWCHAR_TArray(L"./levels/level3.lv");
	world->addLevelFileName(levelFile);
	levelFile = constructEmptyWCHAR_TArray(L"./levels/level2.lv");
	world->addLevelFileName(levelFile);
	/*levelFile = constructEmptyWCHAR_TArray(L"./levels/level4.lv");
	world->addLevelFileName(levelFile);*/

	//start the game loop
	mygame->runGameLoop();

	return 0;
}



/*
	initWRgui - This method builds a GUI for the Empty Game application.
	Note that we load all the GUI components from this method, including
	the ScreenGUI with Buttons and Overlays and the Cursor.
*/
void createGui(Game *game)
{
	GameGraphics *graphics = game->getGraphics();
	GameGUI *gui = game->getGUI();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(200, 100, 200);

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	graphics->setColorKey(102, 153, 204);

	// SETUP THE CURSOR
	vector<int> *imageIDs = new vector<int>();

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	wchar_t *fileName = constructEmptyWCHAR_TArray(L"images/cursor.png");
	int imageID = guiTextureManager->loadTexture(fileName);
	imageIDs->push_back(imageID);

	// - AND NOW THE RED ONE
	fileName = constructEmptyWCHAR_TArray(L"images/cursor.png");
	imageID = guiTextureManager->loadTexture(fileName);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(	imageIDs,
						*(imageIDs->begin()),
						0,
						0,
						0,
						255,
						32,
						32);
	gui->setCursor(cursor);

	// NOW LET'S LOAD A GUI SCREEN, BE CAREFUL TO ADD
	// THESE IN THE PROPER ORDER IF YOU EVER ADD MORE THAN ONE
	ScreenGUI *screenGUI = new ScreenGUI();
	fileName = constructEmptyWCHAR_TArray(L"images/menu.png");
	imageID = guiTextureManager->loadTexture(fileName);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 256;
	imageToAdd->y = 100;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(imageToAdd);

	gui->addScreenGUI(screenGUI);

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();
//BUTTONS
	// Play
	wchar_t* buttonCommand = constructEmptyWCHAR_TArray(START);
	fileName = constructEmptyWCHAR_TArray(L"images/playbtn.png");
	int normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/playbtnovr.png");
	int mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							250,
							0,
							255,
							BTN_WIDTH,
							BTN_HEIGHT,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);
	
	// Controls
	buttonToAdd = new Button();
	buttonCommand = constructEmptyWCHAR_TArray(CTRL);
	fileName = constructEmptyWCHAR_TArray(L"images/ctrlbtn.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/ctrlbtnovr.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							325,
							0,
							255,
							BTN_WIDTH,
							BTN_HEIGHT,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);
	// Help
	buttonToAdd = new Button();
	buttonCommand = constructEmptyWCHAR_TArray(HELP);
	fileName = constructEmptyWCHAR_TArray(L"images/helpbtn.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/helpbtnovr.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							400,
							0,
							255,
							BTN_WIDTH,
							BTN_HEIGHT,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);
	// About
	buttonToAdd = new Button();
	buttonCommand = constructEmptyWCHAR_TArray(ABOUT);
	fileName = constructEmptyWCHAR_TArray(L"images/aboutbtn.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/aboutbtnovr.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							475,
							0,
							255,
							BTN_WIDTH,
							BTN_HEIGHT,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);
	// Exit
	buttonToAdd = new Button();
	buttonCommand = constructEmptyWCHAR_TArray(EXIT);
	fileName = constructEmptyWCHAR_TArray(L"images/exitbtn.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/exitbtnovr.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							550,
							0,
							255,
							BTN_WIDTH,
							BTN_HEIGHT,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);

	// NOW ADD THE IN-GAME GUI
	screenGUI = new ScreenGUI();
	fileName = constructEmptyWCHAR_TArray(L"images/pausebtn.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/pausebtnovr.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);
	buttonCommand = constructEmptyWCHAR_TArray(PAUSE);

	buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							0,
							0,
							0,
							255,
							BTN_WIDTH,
							BTN_HEIGHT,
							false,
							buttonCommand);
	screenGUI->addButton(buttonToAdd);
	gui->addScreenGUI(screenGUI);
}

void pauseGui(Game *game)
{
	GameGraphics *graphics = game->getGraphics();
	GameGUI *gui = game->getGUI();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(200, 100, 200);

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	graphics->setColorKey(102, 153, 204);

	// SETUP THE CURSOR
	vector<int> *imageIDs = new vector<int>();

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	wchar_t *fileName = constructEmptyWCHAR_TArray(L"images/cursor.png");
	int imageID = guiTextureManager->loadTexture(fileName);
	imageIDs->push_back(imageID);

	// - AND NOW THE RED ONE
	fileName = constructEmptyWCHAR_TArray(L"images/cursor.png");
	imageID = guiTextureManager->loadTexture(fileName);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(	imageIDs,
						*(imageIDs->begin()),
						0,
						0,
						0,
						255,
						32,
						32);
	gui->setCursor(cursor);

	// NOW LET'S LOAD A GUI SCREEN, BE CAREFUL TO ADD
	// THESE IN THE PROPER ORDER IF YOU EVER ADD MORE THAN ONE
	ScreenGUI *screenGUI = new ScreenGUI();
	fileName = constructEmptyWCHAR_TArray(L"images/menu.png");
	imageID = guiTextureManager->loadTexture(fileName);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 256;
	imageToAdd->y = 100;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(imageToAdd);

	//MenuButtons
	// New Game
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	wchar_t *buttonCommand = constructEmptyWCHAR_TArray(NEW);
	fileName = constructEmptyWCHAR_TArray(L"images/newbtn.png");
	int normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/newbtnovr.png");
	int mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							225,
							0,
							255,
							PAUSE_WIDTH,
							PAUSE_HEIGHT,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);

	// RESUME
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	buttonCommand = constructEmptyWCHAR_TArray(RESUME);
	fileName = constructEmptyWCHAR_TArray(L"images/resumebtn.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/resumebtnovr.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							300,
							0,
							255,
							PAUSE_WIDTH,
							PAUSE_HEIGHT,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);

	// Controls
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	buttonCommand = constructEmptyWCHAR_TArray(CTRL);
	fileName = constructEmptyWCHAR_TArray(L"images/ctrlbtn.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/ctrlbtnovr.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							375,
							0,
							255,
							PAUSE_WIDTH,
							PAUSE_HEIGHT,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);
	// HELP
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	buttonCommand = constructEmptyWCHAR_TArray(HELP);
	fileName = constructEmptyWCHAR_TArray(L"images/helpbtn.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/helpbtnovr.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							450,
							0,
							255,
							PAUSE_WIDTH,
							PAUSE_HEIGHT,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);
	// ABOUT
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	buttonCommand = constructEmptyWCHAR_TArray(ABOUT);
	fileName = constructEmptyWCHAR_TArray(L"images/aboutbtn.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName=constructEmptyWCHAR_TArray(L"images/aboutbtnovr.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							525,
							0,
							255,
							PAUSE_WIDTH,
							PAUSE_HEIGHT,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);

	// QUIT
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	buttonCommand = constructEmptyWCHAR_TArray(QUIT);
	fileName = constructEmptyWCHAR_TArray(L"images/quitbtn.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/quitbtnovr.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							600,
							0,
							255,
							PAUSE_WIDTH,
							PAUSE_HEIGHT,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);
	gui->addScreenGUI(screenGUI);	
}
void confirmGui(Game *game,wchar_t* conf)
{
	GameGraphics *graphics = game->getGraphics();
	GameGUI *gui = game->getGUI();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(200, 100, 200);

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	graphics->setColorKey(102, 153, 204);

	// SETUP THE CURSOR
	vector<int> *imageIDs = new vector<int>();

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	wchar_t *fileName = constructEmptyWCHAR_TArray(L"images/cursor.png");
	int imageID = guiTextureManager->loadTexture(fileName);
	imageIDs->push_back(imageID);

	// - AND NOW THE RED ONE
	fileName = constructEmptyWCHAR_TArray(L"images/cursor.png");
	imageID = guiTextureManager->loadTexture(fileName);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(	imageIDs,
						*(imageIDs->begin()),
						0,
						0,
						0,
						255,
						32,
						32);
	gui->setCursor(cursor);

	// NOW LET'S LOAD A GUI SCREEN, BE CAREFUL TO ADD
	// THESE IN THE PROPER ORDER IF YOU EVER ADD MORE THAN ONE
	ScreenGUI *screenGUI = new ScreenGUI();
	fileName = constructEmptyWCHAR_TArray(conf);
	imageID = guiTextureManager->loadTexture(fileName);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 256;
	imageToAdd->y = 100;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(imageToAdd);

	

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	wchar_t *buttonCommand = constructEmptyWCHAR_TArray(OK);
	fileName = constructEmptyWCHAR_TArray(L"images/yesbtn.png");
	int normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/yesovr.png");
	int mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							300,
							300,
							0,
							255,
							200,
							100,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	buttonCommand = constructEmptyWCHAR_TArray(CANCEL);
	fileName = constructEmptyWCHAR_TArray(L"images/nobtn.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/noovr.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							550,
							300,
							0,
							255,
							200,
							100,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);

	gui->addScreenGUI(screenGUI);
	
}
void aboutGui(Game *game, wchar_t* menuName)
{
	GameGraphics *graphics = game->getGraphics();
	GameGUI *gui = game->getGUI();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(200, 100, 200);

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	graphics->setColorKey(102, 153, 204);

	// SETUP THE CURSOR
	vector<int> *imageIDs = new vector<int>();

	// - FIRST LOAD THE GREEN CURSOR IMAGE
	wchar_t *fileName = constructEmptyWCHAR_TArray(L"images/cursor.png");
	int imageID = guiTextureManager->loadTexture(fileName);
	imageIDs->push_back(imageID);

	// - AND NOW THE RED ONE
	fileName = constructEmptyWCHAR_TArray(L"images/cursor.png");
	imageID = guiTextureManager->loadTexture(fileName);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(	imageIDs,
						*(imageIDs->begin()),
						0,
						0,
						0,
						255,
						32,
						32);
	gui->setCursor(cursor);

	// NOW LET'S LOAD A GUI SCREEN, BE CAREFUL TO ADD
	// THESE IN THE PROPER ORDER IF YOU EVER ADD MORE THAN ONE
	ScreenGUI *screenGUI = new ScreenGUI();
	fileName = constructEmptyWCHAR_TArray(menuName);
	imageID = guiTextureManager->loadTexture(fileName);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 256;
	imageToAdd->y = 100;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(imageToAdd);

	
	//MenuButtons
	// New Game
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	wchar_t *buttonCommand = constructEmptyWCHAR_TArray(BACK);
	fileName = constructEmptyWCHAR_TArray(L"images/backbtn.png");
	int normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/backbtnovr.png");
	int mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							412,
							700,
							0,
							255,
							PAUSE_WIDTH,
							PAUSE_HEIGHT,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);
	gui->addScreenGUI(screenGUI);	

}

void gameSelectGui(Game *game)
{
	GameGraphics *graphics = game->getGraphics();
	GameGUI *gui = game->getGUI();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(200, 100, 200);

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	graphics->setColorKey(102, 153, 204);

	// NOW LET'S LOAD A GUI SCREEN, BE CAREFUL TO ADD
	// THESE IN THE PROPER ORDER IF YOU EVER ADD MORE THAN ONE
	ScreenGUI *screenGUI = new ScreenGUI();
	wchar_t* fileName = constructEmptyWCHAR_TArray(L"images/gameselectgui.png");
	int imageID = guiTextureManager->loadTexture(fileName);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 256;
	imageToAdd->y = 100;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(imageToAdd);

	

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	wchar_t *buttonCommand = constructEmptyWCHAR_TArray(CTF);
	fileName = constructEmptyWCHAR_TArray(L"images/ctfbtn.png");
	int normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/ctfbtnover.png");
	int mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							300,
							300,
							0,
							255,
							200,
							100,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);

	buttonToAdd = new Button();

	buttonCommand = constructEmptyWCHAR_TArray(ONE);
	fileName = constructEmptyWCHAR_TArray(L"images/onemanbtn.png");
	normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/onemanbtnover.png");
	mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							500,
							300,
							0,
							255,
							200,
							100,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);

	gui->addScreenGUI(screenGUI);
	
}
void gameOverGui(Game *game)
{
	GameGraphics *graphics = game->getGraphics();
	GameGUI *gui = game->getGUI();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(200, 100, 200);

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	graphics->setColorKey(102, 153, 204);

	// NOW LET'S LOAD A GUI SCREEN, BE CAREFUL TO ADD
	// THESE IN THE PROPER ORDER IF YOU EVER ADD MORE THAN ONE
	ScreenGUI *screenGUI = new ScreenGUI();
	wchar_t* fileName = constructEmptyWCHAR_TArray(L"images/gameovergui.png");
	int imageID = guiTextureManager->loadTexture(fileName);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 256;
	imageToAdd->y = 100;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(imageToAdd);

	

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	wchar_t *buttonCommand = constructEmptyWCHAR_TArray(MAINMENU);
	fileName = constructEmptyWCHAR_TArray(L"images/gameOverbtn.png");
	int normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/gameOverbtnovr.png");
	int mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							300,
							300,
							0,
							255,
							200,
							100,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);

	gui->addScreenGUI(screenGUI);
	
}
void gameWinGui(Game *game)
{
	GameGraphics *graphics = game->getGraphics();
	GameGUI *gui = game->getGUI();
	DirectXTextureManager *guiTextureManager = (DirectXTextureManager*)graphics->getGUITextureManager();

	// COLOR USED FOR RENDERING TEXT
	graphics->setFontColor(255, 0, 0);

	// COLOR KEY - COLOR TO BE IGNORED WHEN LOADING AN IMAGE
	graphics->setColorKey(102, 153, 204);

	// NOW LET'S LOAD A GUI SCREEN, BE CAREFUL TO ADD
	// THESE IN THE PROPER ORDER IF YOU EVER ADD MORE THAN ONE
	ScreenGUI *screenGUI = new ScreenGUI();
	wchar_t* fileName = constructEmptyWCHAR_TArray(L"images/gamewingui.png");
	int imageID = guiTextureManager->loadTexture(fileName);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->x = 256;
	imageToAdd->y = 100;
	imageToAdd->z = 0;
	imageToAdd->alpha = 200;
	imageToAdd->width = 512;
	imageToAdd->height = 512;
	imageToAdd->imageID = imageID;
	screenGUI->addOverlayImage(imageToAdd);

	

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	wchar_t *buttonCommand = constructEmptyWCHAR_TArray(MAINMENU);
	fileName = constructEmptyWCHAR_TArray(L"images/gameOverbtn.png");
	int normalTextureID = guiTextureManager->loadTexture(fileName);
	fileName = constructEmptyWCHAR_TArray(L"images/gameOverbtnovr.png");
	int mouseOverTextureID = guiTextureManager->loadTexture(fileName);

	// - INIT THE BUTTON
	buttonToAdd->initButton(normalTextureID, 
							mouseOverTextureID,
							300,
							300,
							0,
							255,
							200,
							100,
							false,
							buttonCommand);

	// AND NOW LOAD IT INTO A ScreenGUI
	screenGUI->addButton(buttonToAdd);

	gui->addScreenGUI(screenGUI);
	
}