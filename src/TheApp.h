/*
 * Copyright 1999 John Foerch, retroj@iserv.net
 * All rights reserved.
 */
#ifndef __THEAPP__
#define __THEAPP__

#include "DeathWindow.h"

#include <Application.h>

class TheApp : public BApplication {
public:
			TheApp();
	virtual bool 	QuitRequested();
private:
	DeathWindow*	fMainWindow;
};

#endif
