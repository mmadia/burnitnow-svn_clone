/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _ABOUTWINDOW_H_
#define _ABOUTWINDOW_H_


#include <Window.h>


// Forward declaration of jpButton class
class jpButton;


class AboutView : public BView {
public:
	AboutView(BRect r, char* name);
	~AboutView();
	virtual void Draw(BRect updateRect);

	BFont* fViewFont;
	BBitmap* fBurnBitmap, *fBurnProofBitmap, *fCDRecordBitmap;
};


class AboutWindow : public BWindow {
public:
	AboutWindow();
	virtual void MessageReceived(BMessage* msg);

	BView* fAroundView;
	AboutView* fAboutView;
	jpButton* fCloseButton;
};


#endif	// _ABOUTWINDOW_H_
