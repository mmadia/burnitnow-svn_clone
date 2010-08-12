/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "CopyCDView.h"

#include "const.h"
#include "jpButton.h"


CopyCDView::CopyCDView(BRect size, BWindow* twin)
	:
	BView(size, "CopyCDView", B_FOLLOW_NONE, B_WILL_DRAW)
{
	SetViewColor(216, 216, 216, 0);
	BRect r;
	r = Bounds();
	r.InsetBy(5.0, 5.0);
	fOpenDirButton = new jpButton(r, "About!", "About", new BMessage(MENU_FILE_ABOUT));

	AddChild(fOpenDirButton);
	fTempWindow = twin;
}


CopyCDView::~CopyCDView()
{
}


void CopyCDView::MessageReceived(BMessage* message)
{
	fTempWindow->MessageReceived(message);
}
