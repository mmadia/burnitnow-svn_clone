#include<Be.h>
#include "jpButton.h"
#include "const.h"
#ifndef __STATUS_WINDOW__
#define __STATUS_WINDOW__
class StatusView : public BView {
	public:
		StatusView(BRect r, char *name);
		virtual void Draw(BRect updateRect);
		virtual void SetAngles(float *ang,int tracks);
		BFont *f1;
		float angles[100];
		int nrtracks;
};

class StatusWindow : public BWindow {
	public:
		StatusWindow(char *title);
		virtual void UpdateStatus(float delta,char *str);
		virtual void MessageReceived(BMessage* msg);
		virtual void StatusSetMax(float t1);
		virtual void StatusSetText(char *str);
		virtual void StatusSetColor(rgb_color color);
		virtual void StatusUpdateReset();
		virtual void SetAngles(float *ang,int tracks);
		virtual void SendMessage(BMessage *msg);
		virtual void Ready();
		BView *Around;
		StatusView *SView;
		jpButton *Close,*More;
		BStatusBar *MyStatus;
		bool fullview;
};
#endif