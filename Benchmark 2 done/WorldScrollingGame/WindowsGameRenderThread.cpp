#include "StdAfx.h"
#include "WindowsGameRenderThread.h"

WindowsGameRenderThread::WindowsGameRenderThread()
{
	renderThreadHandle = NULL;
	timeToProcessData = true;
	timeToRender = false;
	killed=false;
	timeToProcessDataMutex = CreateMutex(0, NULL,
			L"timeToProcessDataMutex");
	timeToRenderMutex = CreateMutex(0, NULL,
			L"timeToRenderMutex");
	renderingRenderListMutex = CreateMutex(0, NULL,
			L"renderingRenderListMutex");
	textureMutex = CreateMutex(0,NULL, L"textureMutex");
	
}
void WindowsGameRenderThread::start(Game *game)
{
	// START THE RENDER THREAD
	renderThreadHandle = (HANDLE)_beginthreadex( NULL, 0, &RenderThread, game, 0, &renderThreadID );
}
void WindowsGameRenderThread::runFromMainThread(Game *game)
{
	WaitForSingleObject(timeToProcessDataMutex, INFINITE);
	if (timeToProcessData)
	{
		timeToProcessData = false;
		ReleaseMutex(timeToProcessDataMutex);
		//game->getInput()->processInput(game);
		WaitForSingleObject(textureMutex, INFINITE);
		game->processGameData();
		
		ReleaseMutex(textureMutex);
		// NOW LET'S SWAP THE RENDER LISTS
		WaitForSingleObject(renderingRenderListMutex, INFINITE);
		GameGraphics *graphics = game->getGraphics();
		graphics->fillRenderLists(game);
		graphics->swapRenderLists();
		ReleaseMutex(renderingRenderListMutex);
		WaitForSingleObject(timeToRenderMutex, INFINITE);
		timeToRender = true;
		ReleaseMutex(timeToRenderMutex);
	}else 
	ReleaseMutex(timeToProcessDataMutex);
}
void WindowsGameRenderThread::kill(){
	killed=true;
}

unsigned __stdcall RenderThread( void* pArguments ){
	Game *game = (Game*)pArguments;
	GameGraphics *graphics = game->getGraphics();
	WindowsGameRenderThread *thread = (WindowsGameRenderThread*)game->getRenderThread();
	while (game->isGameActive()&&!thread->getKilled()){
		WaitForSingleObject(thread->getTimeToRenderMutex(), INFINITE);
		if (thread->isTimeToRender()){
			thread->setTimeToRender(false);
			WaitForSingleObject(thread->getRenderingRenderListMutex(), INFINITE);
			int gameMode = game->getGameState();
			if ((gameMode != LEVEL_LOADING_GAME_MODE)&& (gameMode != LEVEL_UNLOADING_GAME_MODE)
				&& (gameMode!= LEVEL_ALMOST_LOADING_MODE) && (gameMode!=LEVEL_ALMOST_UNLOADING_MODE)){
				WaitForSingleObject(thread->getTextureMutex(),INFINITE);
				graphics->renderGameInRenderThread(game);
				ReleaseMutex(thread->getTextureMutex());
			}
			ReleaseMutex(thread->getRenderingRenderListMutex());
			WaitForSingleObject(thread->getTimeToProcessDataMutex(), INFINITE);
			thread->setTimeToProcessData(true);
			ReleaseMutex(thread->getTimeToProcessDataMutex());
		} else {
			ReleaseMutex(thread->getTimeToRenderMutex());
		}
		ReleaseMutex(thread->getTimeToRenderMutex());
	}
	_endthreadex( 0 );
	return (26);
}