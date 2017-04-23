#include "DeathWindow.h"

#include <Application.h>
#include <Beep.h>
#include <ScrollView.h>
#include <Roster.h>
#include <Messenger.h>
#include <OS.h>

const uint32 KILL_KILL_KILL = 'KiLL';
int32 PIDIndex[10000];


DeathWindow::DeathWindow()
	:
	BWindow(BRect(100, 100, 400, 400), "BeDead", B_TITLED_WINDOW,
		B_NOT_RESIZABLE | B_NOT_ZOOMABLE | B_WILL_ACCEPT_FIRST_CLICK)
{
	DeathList = new BListView(BRect(0, 0, 285, 250), "Death List", B_SINGLE_SELECTION_LIST);

	BScrollView* DeathScroll = new BScrollView("scroll_death", DeathList,
							B_FOLLOW_ALL_SIDES, 0, false, true);

	BMessage* KiLL = new BMessage(KILL_KILL_KILL);
	DeathList->SetInvocationMessage(KiLL);

	CloseCheck = new BCheckBox(BRect(15, 260, 200, 280), "CloseCheck", "Close on Kill", NULL);
	CloseCheck->SetValue(B_CONTROL_ON);
		
	DeathList->AttachedToWindow();
	CloseCheck->AttachedToWindow();

	AddChild(DeathScroll);
	AddChild(CloseCheck);

	AddApps(DeathList);
	
	BMessenger mess(this);
	be_roster->StartWatching(mess);
	
	Show();
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
			DeathList->MakeEmpty();
			AddApps(DeathList);		
			break;

		case B_SOME_APP_QUIT:
			DeathList->MakeEmpty();
			AddApps(DeathList);		
			break;

		case KILL_KILL_KILL:
		{	int32 index;
			message->FindInt32("index", &index);
			KillApp(DeathList, index);
			DeathList->MakeEmpty();
			AddApps(DeathList);	
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
DeathWindow::AddApps(BListView *DeathList)
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
		PIDIndex[lcount] = appInfo.team;
	}
}


void
DeathWindow::KillApp(BListView* DeathList, int32 index)
{
	kill_team(PIDIndex[index]);
	if(CloseCheck->Value())
		be_app->PostMessage(B_QUIT_REQUESTED);
}
