/*
 * Copyright 1999 John Foerch, retroj@iserv.net
 * All rights reserved.
 */
#ifndef DEATH_WINDOW_H
#define DEATH_WINDOW_H

#include <CheckBox.h>
#include <ListView.h>
#include <ScrollView.h>
#include <View.h>
#include <Window.h>

class DeathWindow : public BWindow {
public:
				DeathWindow();
				~DeathWindow();

		virtual void	MessageReceived(BMessage* message);
		virtual bool	QuitRequested();
		
protected:
		void		AddApps(BListView* DeathList);
		void		KillApp(BListView* DeathList, int32 index);
		
		static const uint32 kAppKill = 1000;

		int32		fPIDIndex[10000];
		BListView*	fDeathList;
		BCheckBox*	fCloseCheck;
		BView*		fBackView;
		BScrollView*	fDeathScroll;
};

#endif
