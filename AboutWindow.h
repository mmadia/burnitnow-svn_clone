/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _ABOUTWINDOW_H_
#define _ABOUTWINDOW_H_


#include <Be.h>
#include "jpButton.h"
#include "const.h"


class AboutView : public BView {
public:
	AboutView(BRect r, char* name);
	~AboutView();
	virtual void Draw(BRect updateRect);

	BFont* f1;
	BBitmap* burnbitmap, *burnproof, *cdrecord;
};


class AboutWindow : public BWindow {
public:
	AboutWindow();
	virtual void MessageReceived(BMessage* msg);

	BView* Around;
	AboutView* AView;
	jpButton* Close;
};


#endif	// _ABOUTWINDOW_H_
