#include<Be.h>
#include "jpButton.h"
#ifndef __ASK_NAME__
#define __ASK_NAME__

class AskName : public BWindow {
	public:
		AskName(BRect frame,char *title, uint32 mess, char *what);
		virtual void SendText();
		virtual void MessageReceived(BMessage* msg);
	private:
		BView *Around;
		jpButton *Okey;
		BTextView *Name;
		uint32 message;		
};
#endif