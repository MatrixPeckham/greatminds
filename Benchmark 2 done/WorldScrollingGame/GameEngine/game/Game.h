/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Game.h

	The class is the central object of the game
	application. It plays two important roles:

	1) It manages all major game objects (Game...), which
	essentially manage the game data and communicates with
	external technologies like graphics cards and input
	devices. The objects can be accessed via inlined public
	methods, i.e. getGraphics(), getInput(), getTimer(), etc.

	2) It runs the game loop, calling the appropriate
	methods in the other major objects as needed. The
	runGameLoop method iterates once per frame, invoking
	all necessary methods needed each frame to run the game.

	NOTE: I have deliberately avoided making this class 
	technology specific. I would like to be able to change
	the way the graphics are rendered (i.e. DirectX or OpenGL)
	if I want to without having to change this or most other
	classes.
*/

#pragma once

// PREPROCESSOR INCLUDE STATEMENTS
#include "resource.h"
#include "stdafx.h"
#include <list>
#include <vector>
#include "TextFileWriter.h"

// FORWARD DECLARATIONS OF CLASSES USED BY GAME
class GameAI;
class GameDataLoader;
class GameGraphics;
class GameGUI;
class GameInput;
class GameOS;
class GamePhysics;
class GameText;
class GameTimer;
class GameWorld;
class ButtonEventHandler;
class KeyEventHandler;
class GameRenderThread;
class GameAudio;

// POSSIBLE GAME STATES - WE CAN ADD MORE LATER
#define SPLASH_SCREEN_GAME_STATE	0
#define GAME_PLAYING_GAME_STATE		1
#define PAUSE_GAME_STATE			2
#define CONFIRM_DIALOG_STATE		3
#define ABOUT_GAME_STATE			4
#define HELP_GAME_STATE				5
#define CTRL_GAME_STATE				6
#define NEW_DIALOG_STATE			7
#define GAME_OVER_GAME_STATE		8
#define GAME_SELECT_STATE			9
#define GAME_WIN_STATE				10
#define	EXIT_GAME_STATE				20
#define LEVEL_ALMOST_LOADING_MODE	29
#define LEVEL_LOADING_GAME_MODE		30
#define LEVEL_ALMOST_UNLOADING_MODE	39
#define LEVEL_UNLOADING_GAME_MODE	40

wchar_t* constructEmptyWCHAR_TArray(LPCWSTR initChars);

class Game
{
private:
	// GAME STATE CHANGES WHICH GUI WE DRAW AND WHETHER
	// TO PERFORM GAME LOGIC OR NOT. GAME STATES CAN ALSO
	// TRIGGER LOADING/UNLOADING OF DATA DURING TRANSITIONS
	// FROM ONE STATE TO ANOTHER
	int					gameState;
	wchar_t				*gameDataPathName;
	bool				showLevel;
	// MAJOR GAME OBJECTS
	GameAI				*ai;
	GameDataLoader		*dataLoader;
	GameGraphics		*graphics;
	GameGUI				*gui;
	GameInput			*input;
	GameOS				*os;
	GamePhysics			*physics;
	GameText			*text;
	GameTimer			*timer;
	GameWorld			*world;
	GameRenderThread	*renderThread;
	GameAudio			*music;

	// USED TO PRINT DEBUGGING OUTPUT
	wchar_t *debugLogFileName;
	TextFileWriter *writer;

public:
	// INLINED ACCESSOR METHODS
	int					getGameState()			{ return gameState;							}
	wchar_t*			getGameDataPathName()	{ return gameDataPathName;					}
	bool				isGameActive()			{ return (gameState != EXIT_GAME_STATE);	}
	GameAI*				getAI()					{ return ai;								}
	GameDataLoader*		getDataLoader()			{ return dataLoader;						}
	GameGraphics*		getGraphics()			{ return graphics;							}
	GameGUI*			getGUI()				{ return gui;								}
	GameInput*			getInput()				{ return input;								}
	GameOS*				getOS()					{ return os;								}
	GamePhysics*		getPhysics()			{ return physics;							}
	GameText*			getText()				{ return text;								}
	GameTimer*			getTimer()				{ return timer;								}
	GameWorld*			getWorld()				{ return world;								}
	GameRenderThread*	getRenderThread()		{ return renderThread;						}
	GameAudio*			getGameAudio()			{ return music;								}
	bool				getShowLevel()			{return showLevel;							}
	

	// METHODS DEFINED IN Game.cpp
	Game();
	~Game();
	void	setShowLevel(boolean l) {showLevel=l;}
	void	setGameAudio(GameAudio* m) {
		delete music;
		music=m;
	}
	void	setPhysics(GamePhysics* p) {
		delete physics;
		physics=p;
	}
	void	setInput(GameInput* p) {
		input=p;
	}
	void	changeGameState(int initGameState);
	void	init(	wchar_t *initGameDataPathName, 
					GameGraphics *initGraphics, 
					GameOS *initOS, 
					GameInput *initInput,
					GameTimer *initTimer,
					GameRenderThread *initThread);
	void	killGameApplication();
	void	processGameData();
	void	processGameLogic();
	void	reloadAllDevices();
	void	runGameLoop();
	void	shutdownEverything();
	void	writeOutput(wchar_t *output);
};