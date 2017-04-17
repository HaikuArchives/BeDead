#include <Application.h>
#include <Window.h>
#include <ListView.h>
#include <ScrollView.h>
#include <Roster.h>

#include "TheApp.h"
#include "DeathWindow.h"

const char* APP_SIGNATURE="application/x-vnd.BeDead";

/////////////////////////////////////////////////
// Constructor.... nothing to see here, people...
// move along...
//
TheApp::TheApp() : BApplication(APP_SIGNATURE) {
}

//////////////////////////////////////////////////
// ReadyToRun.. this function is called once
// initial application stuff is all done.. real
// execution begins here... probably ends here, too
//
void TheApp::ReadyToRun() {

	new DeathWindow();
}

//////////////////////////////////////////////////
// Application receives a B_QUIT_REQUESTED,
// Pass the message on to all the windows
//
bool TheApp::QuitRequested() {
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}

//////////////////////////////////////////////////
// the UBIQUITOUS main() .. sets up the application
// loop... no other function
//
int main() {
	TheApp a;
	a.Run();
	return 0;
}