/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "jpButton.h"


jpButton::jpButton(BRect frame, char* name, char* title, BMessage* msg, uint32 resizingMode = B_FOLLOW_LEFT | B_FOLLOW_TOP, uint32 flags = B_WILL_DRAW | B_NAVIGABLE)
	:
	BControl(frame, name, title, msg, resizingMode, flags)
{
	strcpy(fTitle, title);
	SetFont(be_plain_font);
	fEnabled = true;
	fDown = false;
}


jpButton::~jpButton()
{
}


void jpButton::SetTitle(char* title)
{
	strcpy(fTitle, title);
	Invalidate();
}


void jpButton::Draw(BRect updateRect)
{
	font_height fh;
	BRect r;
	r = Bounds();
	SetPenSize(1.0);
	if (fEnabled) {
		if (fDown) {
			SetHighColor(190, 190, 190);
			FillRect(r);
			SetHighColor(100, 100, 100);
			StrokeRect(r);
			SetHighColor(0, 0, 0);
		} else {
			SetHighColor(227, 227, 227);
			FillRect(r);
			SetHighColor(160, 160, 160);
			StrokeRect(r);
			SetHighColor(0, 0, 0);
		}
	} else {
		SetHighColor(227, 227, 227);
		FillRect(r);
		SetHighColor(180, 180, 180);
		StrokeRect(r);
		SetHighColor(120, 120, 120);
	}

	be_plain_font->GetHeight(&fh);
	MovePenTo((Bounds().right / 2) - ((be_plain_font->StringWidth(fTitle) / 2)), (Bounds().bottom / 2) + (fh.ascent / 2));
	DrawString(fTitle);
	if (BControl::IsFocus() && fEnabled) {
		SetHighColor(120, 120, 120);
		BPoint p1, p2;
		p1.x = (Bounds().right / 2) - ((be_plain_font->StringWidth(fTitle) / 2)) - 1;
		p1.y = (Bounds().bottom / 2) + (fh.ascent / 2) + 1;
		p2.x =  p1.x + be_plain_font->StringWidth(fTitle);
		p2.y = p1.y;
		StrokeLine(p1, p2);
	}
	BControl::Draw(updateRect);
}


void jpButton::MouseDown(BPoint p)
{
	if (fEnabled) {
		fDown = true;
		Invalidate();
	}
}


void jpButton::MouseUp(BPoint p)
{
	if (fEnabled) {
		fDown = false;
		Invalidate();
		Invoke();
	}
}


void jpButton::MouseMoved(BPoint p, uint32 code, const BMessage* a_message)
{
	BPoint ptemp;
	uint32 buttons;

	if (code == B_EXITED_VIEW)
		if (fEnabled && fDown) {
			fDown = false;
			Invalidate();
		}
	if (code == B_ENTERED_VIEW) {

		GetMouse(&ptemp, &buttons);
		if (buttons) {
			MouseDown(p);
		}
	}
}


void jpButton::SetEnabled(bool what)
{
	fEnabled = what;
	Invalidate();
}


void jpButton::MakeFocus(bool focusState = true)
{
	BControl::MakeFocus(focusState);
	Invalidate();
}


void jpButton::KeyDown(const char* bytes, int32 numBytes)
{
	if (BControl::IsFocus() && fEnabled && bytes[0] == B_SPACE) {
		fDown = true;
		Invalidate();
	}
	BControl::KeyDown(bytes, numBytes);
}


void jpButton::KeyUp(const char* bytes, int32 numBytes)
{
	if (BControl::IsFocus() && fEnabled && bytes[0] == B_SPACE) {
		fDown = false;
		Invalidate();
		Invoke();
	}
	BControl::KeyUp(bytes, numBytes);
}
