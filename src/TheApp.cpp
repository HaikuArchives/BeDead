/*
 * Copyright 1999 John Foerch, retroj@iserv.net
 * All rights reserved.
 */

#include "TheApp.h"

#include <Roster.h>
#include <ScrollView.h>

#include "DeathWindow.h"

const char* kAppSignature = "application/x-vnd.BeDead";


TheApp::TheApp()
	:
	BApplication(kAppSignature)
{
	fMainWindow = new DeathWindow();
	fMainWindow->Show();
}


bool
TheApp::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


int
main()
{
	TheApp a;
	a.Run();
	return 0;
}
