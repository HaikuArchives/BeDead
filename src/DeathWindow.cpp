/*
 * Copyright 1999 John Foerch, retroj@iserv.net
 * All rights reserved.
 */

#include "DeathWindow.h"

#include <Application.h>
#include <Beep.h>
#include <LayoutBuilder.h>
#include <Messenger.h>
#include <OS.h>
#include <Roster.h>


DeathWindow::DeathWindow()
	:
	BWindow(BRect(), "BeDead", B_TITLED_WINDOW,
		B_AUTO_UPDATE_SIZE_LIMITS | B_WILL_ACCEPT_FIRST_CLICK)
{
	ResizeTo(300, 280);
	CenterOnScreen();

	fBackView = new BView("BackView", B_WILL_DRAW);
	rgb_color bgcolor = ui_color(B_PANEL_BACKGROUND_COLOR);
	fBackView->SetViewColor(bgcolor);

	fDeathList = new BListView("Death List", B_SINGLE_SELECTION_LIST,
		B_WILL_DRAW);

	fDeathScroll = new BScrollView("scroll_death", fDeathList,
		B_WILL_DRAW, false, true);
	fDeathScroll->SetExplicitMinSize(BSize(260, 260));

	fDeathList->SetInvocationMessage(new BMessage(kAppKill));

	fCloseCheck = new BCheckBox("CloseCheck", "Close BeDead on Kill", NULL, NULL);
	fCloseCheck->SetValue(B_CONTROL_OFF);

	BLayoutBuilder::Group<>(fBackView, B_VERTICAL)
		.Add(fDeathScroll)
		.AddGroup(B_HORIZONTAL)
			.Add(fCloseCheck)
			.AddGlue()
		.End()
		.End();

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.AddGroup(B_VERTICAL)
			.SetInsets(B_USE_WINDOW_SPACING)
			.Add(fBackView)
			.End();

	AddApps(fDeathList);
	
	BMessenger mess(this);
	be_roster->StartWatching(mess);
	
}


DeathWindow::~DeathWindow()
{
	BMessenger mess(this);
	be_roster->StopWatching(mess);
}


void
DeathWindow::MessageReceived(BMessage *message)
{
	switch(message->what) {
		case B_SOME_APP_LAUNCHED:
		{
			fDeathList->MakeEmpty();
			AddApps(fDeathList);
			break;
		}

		case B_SOME_APP_QUIT:
		{
			fDeathList->MakeEmpty();
			AddApps(fDeathList);
			break;
		}

		case kAppKill:
		{
			int32 index;
			message->FindInt32("index", &index);
			KillApp(fDeathList, index);
			fDeathList->MakeEmpty();
			AddApps(fDeathList);
			break;
		}

		default:
			BWindow::MessageReceived(message);
			break;
		}
}		


bool
DeathWindow::QuitRequested()
{
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


void
DeathWindow::AddApps(BListView* DeathList)
{
	BList* PIDList = new BList;
	
	be_roster->GetAppList(PIDList);
	int32 lcount, count = PIDList->CountItems() - 1;
	app_info appInfo;
	team_id team;
	for(lcount = 0; lcount <= count; lcount++) {
		team = (team_id)PIDList->ItemAt(lcount);
		be_roster->GetRunningAppInfo(team, &appInfo);
		DeathList->AddItem(new BStringItem(appInfo.signature));
		fPIDIndex[lcount] = appInfo.team;
	}
}


void
DeathWindow::KillApp(BListView* DeathList, int32 index)
{
	kill_team(fPIDIndex[index]);
	if (fCloseCheck->Value())
		be_app->PostMessage(B_QUIT_REQUESTED);
}
