/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include<Be.h>
#include "jpButton.h"
#include "const.h"
#ifndef __ABOUT_WINDOW__
#define __ABOUT_WINDOW__
class AboutView : public BView {
	public:
		AboutView(BRect r, char *name);
		~AboutView();
		virtual void Draw(BRect updateRect);
		BFont *f1;
		BBitmap *burnbitmap,*burnproof,*cdrecord;

};

class AboutWindow : public BWindow {
	public:
		AboutWindow();
		virtual void MessageReceived(BMessage* msg);
		BView *Around;
		AboutView *AView;
		jpButton *Close;
		
};
#endif
