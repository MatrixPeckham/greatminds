/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	Viewport.h

	This class stores information about the viewport to help
	with rendering. The viewport may move around the game world
	by calling moveViewport, which changes viewportX and 
	viewportY. It also has information about the size of the 
	viewport. Note that the viewport width and height refer
	to the full rendering area of the game, including any GUI
	regions. To compute the size of the game rendering area,
	which might not include GUI toolbars, for example, one may
	subtract the viewport offsets from the width and height
	values.
*/
#pragma once
#include "resource.h"
#include "stdafx.h"
#include "Game.h"

class Viewport
{
private:
	bool autoScrollX;
	bool autoScrollY;
	int scrollSpeedX;
	int scrollSpeedY;
	int viewportX;
	int viewportY;
	int viewportWidth;
	int viewportHeight;
	int viewportOffsetX;
	int viewportOffsetY;

public:
	// INLINED ACCESSOR METHODS
	bool getAutoScrollX()       {return autoScrollX;			}
	bool getAutoScrollY()       {return autoScrollY;			}
	int getScrollSpeedX()		{ return scrollSpeedX;			}
	int getScrollSpeedY()		{ return scrollSpeedY;			}
	int getViewportX()			{ return viewportX;				}
	int getViewportY()			{ return viewportY;				}
	int getViewportWidth()		{ return viewportWidth;			}
	int getViewportHeight()		{ return viewportHeight;		}
	int getViewportOffsetX()	{ return viewportOffsetX;		}
	int getViewportOffsetY()	{ return viewportOffsetY;		}

	// INLINED MUTATOR METHODS
	void setScrollSpeedX(int initScrollSpeedX)
	{ scrollSpeedX = initScrollSpeedX;		}
	void setScrollSpeedY(int initScrollSpeedY)
	{ scrollSpeedY = initScrollSpeedY;		}
	void setViewportX(int initViewportX)				
	{ viewportX = initViewportX;			}
	void setViewportY(int initViewportY)				
	{ viewportY = initViewportY;			}
	void setViewportWidth(int initViewportWidth)
	{ viewportWidth = initViewportWidth;	}
	void setViewportHeight(int initViewportHeight)
	{ viewportHeight = initViewportHeight;	}
	void setViewportOffsetX(int initViewportOffsetX)	
	{ viewportOffsetX = initViewportOffsetX;}
	void setViewportOffsetY(int initViewportOffsetY)	
	{ viewportOffsetY = initViewportOffsetY;}
	void setAutoScroll(bool x, bool y) {
		autoScrollX=x;
		autoScrollY=y;
	}
	// METHODS DEFINED IN Viewport.cpp
	Viewport();
	~Viewport();
	bool areWorldCoordinatesInViewport(int x, int y, int width, int height);
	bool areViewportCoordinatesInViewport(int x, int y, int width, int height);
	void moveViewport(int incX, int incY,	int worldWidth, int worldHeight);
	void checkAutoScroll(int w, int h);
	void checkAutoScroll(int w, int h, int speed);
};