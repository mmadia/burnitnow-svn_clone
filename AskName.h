/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _ASKNAME_H_
#define _ASKNAME_H_


#include <Be.h>
#include "jpButton.h"


class AskName : public BWindow {
public:
	AskName(BRect frame, char* title, uint32 mess, char* what);
	virtual void SendText();
	virtual void MessageReceived(BMessage* msg);

private:
	BView* Around;
	jpButton* Okey;
	BTextView* Name;
	uint32 message;
};


#endif	// _ASKNAME_H_
