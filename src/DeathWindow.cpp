#include "DeathWindow.h"

#include <Application.h>
#include <Beep.h>
#include <View.h>
#include <LayoutBuilder.h>
#include <Messenger.h>
#include <OS.h>
#include <Roster.h>
#include <ScrollView.h>

const uint32 KILL_KILL_KILL = 'KiLL';
int32 PIDIndex[10000];


DeathWindow::DeathWindow()
	:
	BWindow(BRect(), "BeDead", B_TITLED_WINDOW,
			 B_AUTO_UPDATE_SIZE_LIMITS | B_WILL_ACCEPT_FIRST_CLICK)
{
	ResizeTo(300, 280);
	CenterOnScreen();

	BView* fBackView = new BView("BackView", B_WILL_DRAW);
	rgb_color bgcolor = ui_color(B_PANEL_BACKGROUND_COLOR);
	fBackView->SetViewColor(bgcolor);

	DeathList = new BListView("Death List", B_SINGLE_SELECTION_LIST,
					B_WILL_DRAW);

	BScrollView* DeathScroll = new BScrollView("scroll_death", DeathList,
												B_WILL_DRAW, false, true);
	DeathScroll->SetExplicitMinSize(BSize(260, 260));

	BMessage* KiLL = new BMessage(KILL_KILL_KILL);
	DeathList->SetInvocationMessage(KiLL);

	CloseCheck = new BCheckBox("CloseCheck", "Close BeDead on Kill", NULL, NULL);
	CloseCheck->SetValue(B_CONTROL_OFF);
		
	DeathList->AttachedToWindow();
	CloseCheck->AttachedToWindow();

	BLayoutBuilder::Group<>(fBackView, B_VERTICAL)
		.Add(DeathScroll)
		.AddGroup(B_HORIZONTAL)
			.Add(CloseCheck)
			.AddGlue()
		.End()
		.End();

	BLayoutBuilder::Group<>(this, B_VERTICAL, 0)
		.AddGroup(B_VERTICAL)
			.SetInsets(B_USE_WINDOW_SPACING)
			.Add(fBackView)
			.End();

	AddApps(DeathList);
	
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
