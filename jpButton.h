/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _JPBUTTON_H_
#define _JPBUTTON_H_


#include <Control.h>


class jpButton : public BControl {
public:
	jpButton(BRect frame, char* name, char* title, BMessage* msg, uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP, uint32 flags = B_WILL_DRAW | B_NAVIGABLE);
	~jpButton();
	virtual void MouseDown(BPoint p);
	virtual void MouseUp(BPoint p);
	virtual void MouseMoved(BPoint p, uint32 code, const BMessage* a_message);

	virtual void SetTitle(char* title);
	virtual void Draw(BRect updateRect);
	virtual void SetEnabled(bool what);
	virtual void MakeFocus(bool focusState = true);
	virtual void KeyDown(const char* bytes, int32 numBytes);
	virtual void KeyUp(const char* bytes, int32 numBytes);

	bool fDown;
	bool fEnabled;
	char fTitle[100];
};


#endif	// _JPBUTTON_H_
