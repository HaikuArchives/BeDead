
#ifndef __THEAPP__
#define __THEAPP__

class TheApp : public BApplication {
public:
	TheApp();
	virtual void ReadyToRun();
	virtual bool QuitRequested();
};

#endif