/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "AboutWindow.h"

#include "const.h"

#include <Button.h>
#include <Bitmap.h>


#define  ABOUT_STRING "BurnItNow beta 3"
#define ABOUT_STRING2 "©2000-2002 Johan Nilsson"
#define ABOUT_STRING3 "©2010 BurnItNow Maintainers"


BBitmap* GetBitmapResource(type_code type, const char* name);


AboutView::AboutView(BRect r, const char* title)
	:
	BView(r, title, B_FOLLOW_NONE, B_WILL_DRAW)
{
	fViewFont = new BFont(be_plain_font);
	SetFont(fViewFont);
	fBurnBitmap = GetBitmapResource('BBMP', "BMP:BACKGROUND");
	fBurnProofBitmap = GetBitmapResource('BBMP', "BMP:BURNPROOF");
	fCDRecordBitmap = GetBitmapResource('BBMP', "BMP:CDRECORD");
}


AboutView::~AboutView()
{
	delete fViewFont;
	delete fBurnBitmap;
	delete fBurnProofBitmap;
	delete fCDRecordBitmap;
}


void AboutView::Draw(BRect updateRect)
{
	SetLowColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	SetHighColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	BRect r;
	BPoint p1;
	r = Bounds();
	FillRect(r);
	p1.x = (r.right / 2) - 32;
	p1.y = r.top + 12;
	DrawBitmap(fBurnBitmap, p1);
	fViewFont->SetSize(15);
	fViewFont->SetFace(B_BOLD_FACE);
	SetFont(fViewFont);
	p1.x = (r.right / 2) - (fViewFont->StringWidth(ABOUT_STRING) / 2);
	p1.y = r.top + 20;
	MovePenTo(p1);
	SetHighColor(0, 0, 0);
	DrawString(ABOUT_STRING);
	fViewFont->SetSize(10);
	fViewFont->SetFace(B_REGULAR_FACE);
	SetFont(fViewFont);
	p1.x = (r.right / 2) - (fViewFont->StringWidth(ABOUT_STRING2) / 2);
	p1.y += 60;
	MovePenTo(p1);
	SetHighColor(0, 0, 0);
	DrawString(ABOUT_STRING2);
	// p1.x = r.left + 100;
	p1.y += 14;
	MovePenTo(p1);
	SetHighColor(0, 0, 0);
	DrawString(ABOUT_STRING3);
	p1.x = r.left + 100;
	p1.y = r.top + 100;
	DrawBitmap(fBurnProofBitmap, p1);
	p1.x = r.left + 15;
	p1.y = r.top + 100; // was 70
	DrawBitmap(fCDRecordBitmap, p1);
}


AboutWindow::AboutWindow()
	:
	BWindow(BRect(250, 200, 460, 380), "AboutWindow", B_MODAL_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL, B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_NOT_CLOSABLE)
{
	BRect r;   // was 250, 200, 460, 350
	SetTitle("About");

	r = Bounds();
	BView* aroundView = new BView(r, "AroundView", B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	aroundView->SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));
	AddChild(aroundView);

	r = aroundView->Bounds();
	r.InsetBy(10.0, 10.0);
	r.top = r.bottom - 20;
	r.left = r.right - 55;
	aroundView->AddChild(new BButton(r, "close", "Close", new BMessage('ClWi')));
	r = aroundView->Bounds();
	r.InsetBy(5.0, 5.0);
	r.bottom = r.bottom - 25;
	aroundView->AddChild(new AboutView(r, "AboutView"));
}


void AboutWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case 'ClWi':
			Quit();
			break;
	}
}
