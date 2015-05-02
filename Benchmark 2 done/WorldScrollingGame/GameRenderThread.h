#include "Game.h"
class GameRenderThread
{
	public:
		virtual void kill()=0;
//		virtual void suspend();
//		virtual void resume();
		virtual void runFromMainThread(Game *game)=0;
		virtual void start(Game *game)=0;
};
