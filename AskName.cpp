/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "AskName.h"

#include "const.h"

#include <Application.h>
#include <Button.h>
#include <ScrollView.h>


AskName::AskName(BRect frame, char* title, uint32 mess, char* what)
	:
	BWindow(frame, "AskName", B_FLOATING_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_ZOOMABLE | B_NOT_RESIZABLE)
{
	BRect r, r2;
	if (title != NULL)
		SetTitle(title);
	else
		SetTitle("BurnItNow");

	fMessageWhat = mess;

	r = Bounds();
	fAroundView = new BView(r, "Around", B_FOLLOW_NONE, B_WILL_DRAW);
	fAroundView->SetViewColor(216, 216, 216, 0);
	AddChild(fAroundView);

	r = fAroundView->Bounds();
	r.InsetBy(5.0, 5.0);
	r.right -= 70;
	r.bottom = 23;
	fNameTextView = new BTextView(r, "Name_", r, B_FOLLOW_NONE, B_WILL_DRAW);
	fNameTextView->SetWordWrap(false);
	fNameTextView->DisallowChar('\n');
	if (fMessageWhat == VOLUME_NAME) {
		//Name->DisallowChar(' ');
	}
	fAroundView->AddChild(new BScrollView("NameScroll", fNameTextView, B_FOLLOW_NONE, 0, false, false, B_FANCY_BORDER));
	r2 = fAroundView->Bounds();
	r2.InsetBy(5.0, 5.0);
	r2.left = r.right + 5;
	r2.top -= 3;
	r2.bottom = 25;
	fOkButton = new BButton(r2, "Ok", "Ok", new BMessage('STxt'));
	fAroundView->AddChild(fOkButton);

	fNameTextView->MakeFocus();
	if (what != NULL) {
		fNameTextView->Insert(what);
		fNameTextView->SelectAll();
	}
}


void AskName::SendText()
{
	BMessage* temp_msg;
	temp_msg = new BMessage(fMessageWhat);
	temp_msg->AddString("DirName", fNameTextView->Text());
	be_app->PostMessage(temp_msg);
	Quit();
}


void AskName::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case 'STxt' :
			SendText();
			break;
		default :
			BWindow::MessageReceived(msg);
	}
}
