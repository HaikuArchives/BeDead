#include <CheckBox.h>
#include <ListView.h>
#include <Window.h>

class DeathWindow : public BWindow {

	public:
		DeathWindow();
		~DeathWindow();
		virtual void MessageReceived(BMessage *message);
		virtual bool QuitRequested();
		
	protected:
		void AddApps(BListView *DeathList);
		void KillApp(BListView *DeathList,int32 index);
		
		BListView *DeathList;
		BCheckBox *CloseCheck;

};
