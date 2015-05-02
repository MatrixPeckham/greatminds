/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	WRTextGenerator.cpp

	See WRTextGenerator.h for a class description.
*/

#include "stdafx.h"
#include "MyTextClass.h"
#include "Game.h"
#include "SpriteManager.h"
#include "Player.h"
#include "GameGraphics.h"
#include "GameText.h"
#include "GameTimer.h"
#include "GameAI.h"
#include "WindowsGameTimer.h"

extern int GAME_MODE;
wchar_t welcomeMessage[40];
wchar_t hp[7];
wchar_t ammo[20];
wchar_t bombs[20];
wchar_t weapon[30];
wchar_t ai[30];
size_t counter = 0;

void WRTextGenerator::initText(Game *game)
{
	GameText *text = game->getText();
	WindowsGameTimer *timer = (WindowsGameTimer*)game->getTimer();
	welcomeMessage[0] = '\0';
	hp[0] = '\0';
	ammo[0] = '\0';
	bombs[0] = '\0';
	ai[0]='\0';
	weapon[0]='\0';
	swprintf(welcomeMessage, L"FPS: %d", timer->getTargetFPS());
	text->addText(welcomeMessage, 800, 0, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT);
	text->addText(hp,350,0,DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT+10);
	text->addText(ammo,350,20,DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT+10);
	text->addText(bombs,500,0,DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT+10);
	text->addText(weapon,500,20,DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT+10);
	if(GAME_MODE==1) {
		text->addText(ai,400,40,DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT+10);
	}
}

void WRTextGenerator::updateText(Game *game)
{
	// GET THE text OBJECT IF YOU LIKE
	GameText *text = game->getText();
	WindowsGameTimer *timer = (WindowsGameTimer*)game->getTimer();
	Player* p=((Player*)game->getWorld()->getSpriteManager()->getPlayer());
	// UPDATE AND DISPLAY ANY TEXT WE WANT, WE KNOW THAT
	// GameText IS RENDERING THIS ONE, SINCE WE GAVE IT THE
	// MEMORY ADDRESS, SO WE CAN JUST CHANGE THIS ONE DIRECTLY
	welcomeMessage[0] = '\0';
	hp[0] = '\0';
	ammo[0] = '\0';
	bombs[0] = '\0';
	ai[0] = '\0';
	weapon[0]='\0';
	swprintf(welcomeMessage, L"FPS: %d %d", timer->getTargetFPS(), timer->getLoopCounter());
	if(game->getGameState()==GAME_PLAYING_GAME_STATE) {
		swprintf(hp, L"HP: %d", p->hp);
		swprintf(ammo, L"Ammo: %d", p->ammo);
		swprintf(bombs, L"Bombs: %d", p->bombs);
		swprintf(weapon, p->weaponToString());
		int color=(int)(255*((double)p->hp/100));
			game->getGraphics()->setFontColor(255, color, color);

		if(GAME_MODE==1) {
			swprintf(ai, L"Enemies Remaining: %d", game->getAI()->getAIRemaining());
		}
	}
	
	counter++;
}