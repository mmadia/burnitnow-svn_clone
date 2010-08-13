/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "CopyCDView.h"

#include "const.h"

#include <Button.h>


CopyCDView::CopyCDView(BRect size, BWindow* twin)
	:
	BView(size, "CopyCDView", B_FOLLOW_NONE, B_WILL_DRAW)
{
	SetViewColor(216, 216, 216, 0);
	BRect r;
	r = Bounds();
	r.InsetBy(5.0, 5.0);

	AddChild(new BButton(r, "About!", "About", new BMessage(MENU_FILE_ABOUT)));
	fTempWindow = twin;
}


CopyCDView::~CopyCDView()
{
}


void CopyCDView::MessageReceived(BMessage* message)
{
	fTempWindow->MessageReceived(message);
}
