/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _STATUSWINDOW_H_
#define _STATUSWINDOW_H_


#include <StatusBar.h>
#include <Window.h>


// Forward declaration of jpButton class
class jpButton;


class StatusView : public BView {
public:
	StatusView(BRect r, char* name);
	virtual void Draw(BRect updateRect);
	virtual void SetAngles(float* ang, int tracks);

	BFont* f1;
	float angles[100];
	int nrtracks;
};


class StatusWindow : public BWindow {
public:
	StatusWindow(char* title);
	virtual void UpdateStatus(float delta, char* str);
	virtual void MessageReceived(BMessage* msg);
	virtual void StatusSetMax(float t1);
	virtual void StatusSetText(char* str);
	virtual void StatusSetColor(rgb_color color);
	virtual void StatusUpdateReset();
	virtual void SetAngles(float* ang, int tracks);
	virtual void SendMessage(BMessage* msg);
	virtual void Ready();

	BView* Around;
	StatusView* SView;
	jpButton* Close, *More;
	BStatusBar* MyStatus;
	bool fullview;
};


#endif	// _STATUSWINDOW_H_
