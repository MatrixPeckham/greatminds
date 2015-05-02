#pragma once
#include "resource.h"
#include "stdafx.h"
#include "Game.h"
#include "GameGraphics.h"
#include "gamerenderthread.h"

unsigned __stdcall RenderThread( void* pArguments );

class WindowsGameRenderThread : public GameRenderThread
{
	private:
		HANDLE renderThreadHandle;
		unsigned int renderThreadID;
		Game *game;

		bool timeToProcessData;
		bool timeToRender;

		HANDLE timeToProcessDataMutex;
		HANDLE timeToRenderMutex;
		HANDLE renderingRenderListMutex;
		HANDLE textureMutex;
		bool killed;
	public:
		WindowsGameRenderThread();
		void runFromMainThread(Game *game);
		void start(Game *game);
		HANDLE getRenderingRenderListMutex(){return renderingRenderListMutex;}
		HANDLE getTimeToRenderMutex(){return timeToRenderMutex;}
		bool isTimeToRender(){return timeToRender;}
		void setTimeToRender(bool initTime){timeToRender=initTime;}
		HANDLE getTimeToProcessDataMutex(){return timeToProcessDataMutex;}
		HANDLE getTextureMutex(){return textureMutex;}
		bool getTimeToProcessData(){return timeToProcessData;}
		void setTimeToProcessData(bool initTime){timeToProcessData=initTime;}
		bool getKilled(){return killed;}
		void kill();
};

