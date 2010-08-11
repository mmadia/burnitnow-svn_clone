/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "StatusWindow.h"
#include "const.h"


extern char VOL_NAME[25];
int32 TRACK_FIN;


StatusView::StatusView(BRect r, char* title)
	:
	BView(r, title, B_FOLLOW_NONE, B_WILL_DRAW)
{
	f1 = new BFont(be_plain_font);
	SetFont(f1);
	nrtracks = 0;
	angles[0] = 0;
	TRACK_FIN = 0;
}


void StatusView::SetAngles(float ang[100], int tracks)
{
	int i;
	nrtracks = tracks;
	for (i = 0; i < nrtracks; i++)
		angles[i] = ang[i];
}


void StatusView::Draw(BRect updateRect)
{
	char temp[5];
	float oldangle;
	int i;
	BPoint center, p1;
	SetLowColor(216, 216, 216);
	f1->SetSize(13);
	center.x = 70;
	center.y = Bounds().bottom / 2;
	SetHighColor(216, 216, 216);
	FillRect(Bounds());
	SetHighColor(190, 190, 190);
	SetPenSize(2.0);
	FillEllipse(center, 60.0, 60.0);
	SetHighColor(50, 50, 50);
	SetPenSize(2.0);
	StrokeEllipse(center, 60.0, 60.0);

	SetHighColor(105, 105, 105);

	oldangle = 0;
	SetLowColor(190, 190, 190);
	if (nrtracks > 1)
		for (i = 0; i < nrtracks; i++) {
			p1 = center;
			p1.y += 60 * sin((3.14 / 180) * (angles[i] - 90));
			p1.x += 60 * cos((3.14 / 180) * (angles[i] - 90));
			StrokeLine(center, p1);
			p1 = center;
			p1.y += 40 * sin((3.14 / 180) * ((angles[i] - (angles[i] - oldangle) / 2) - 85));
			p1.x += 40 * cos((3.14 / 180) * ((angles[i] - (angles[i] - oldangle) / 2) - 85));
			MovePenTo(p1);
			f1->SetRotation(-(angles[i] - (angles[i] - oldangle) / 2) - 180 - 89);
			if (i < TRACK_FIN) {
				f1->SetFace(B_BOLD_FACE);
				SetFont(f1);

				if (TRACK_FIN - 1 == i)
					SetHighColor(255, 0, 0);
				else
					SetHighColor(0, 0, 0);

				sprintf(temp, "[%d]", i + 1);
			} else {
				f1->SetFace(B_REGULAR_FACE);
				SetFont(f1);
				SetHighColor(0, 0, 0);
				sprintf(temp, "%d", i + 1);
			}
			DrawString(temp);
			SetHighColor(105, 105, 105);
			oldangle = angles[i];
		}

	else {
		if (nrtracks > 0) {

			SetHighColor(0, 0, 0);
			SetFont(f1);
			MovePenTo(center);
			DrawString("1");
		}
	}
	SetHighColor(50, 50, 50);
	p1 = center;
	p1.y -= 60;
}


StatusWindow::StatusWindow(char* title)
	:
	BWindow(BRect(220, 200, 500, 270), "StatusWindow", B_FLOATING_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL, B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_NOT_CLOSABLE)
{
	BRect r;
	if (title != NULL)
		SetTitle(title);
	else
		SetTitle("BurnItNow");

	r = Bounds();
	Around = new BView(r, "Around", B_FOLLOW_ALL_SIDES, B_WILL_DRAW);
	Around->SetViewColor(216, 216, 216, 0);
	AddChild(Around);

	r = Around->Bounds();
	r.InsetBy(5.0, 5.0);
	r.bottom = r.top + 30;
	MyStatus = new BStatusBar(r, "MyStatus");
	Around->AddChild(MyStatus);
	MyStatus->SetText("");

	fullview = false;

	r = Around->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = r.bottom - 20;
	r.left = r.right - 35;
	Close = new jpButton(r, "close", "Close", new BMessage('ClWi'));
	Around->AddChild(Close);
	Close->SetEnabled(false);
	r.right = r.left - 5;
	r.left = r.right - 35;
	More = new jpButton(r, "more", "More..", new BMessage('More'));
	Around->AddChild(More);
	r = Around->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 70;
	r.bottom = 210;
	SView = new StatusView(r, "StatusView");
	Around->AddChild(SView);
}


void StatusWindow::SendMessage(BMessage* msg)
{
	be_app->PostMessage(msg);
}


void StatusWindow::StatusSetMax(float t1)
{
	MyStatus->SetMaxValue(t1);
}


void StatusWindow::UpdateStatus(float delta, char* str)
{
	char temp[1024];
	char temp_char[5];
	int temp_int;
	memset(temp_char, 0, 5);

	strncpy(temp_char, &str[6], 2);
	temp_int = atol(temp_char);
	if (temp_int != TRACK_FIN) {
		TRACK_FIN = temp_int;
		SView->Invalidate();
	}
	MyStatus->Update(delta - MyStatus->CurrentValue(), str);
	sprintf(temp, "%s - [%s]", VOL_NAME, str);
	SetTitle(temp);
}


void StatusWindow::StatusSetText(char* str)
{
	char temp[1024];
	char temp_char[5];

	strncpy(temp_char, &str[7], 2);
	TRACK_FIN = atol(temp_char);
	if (!strcmp("Fixating...", str))
		TRACK_FIN = 1000;
	SView->Invalidate();
	sprintf(temp, "%s - [%s]", VOL_NAME, str);
	MyStatus->SetText(str);
	SetTitle(temp);
}


void StatusWindow::StatusSetColor(rgb_color color)
{
	MyStatus->SetBarColor(color);
}


void StatusWindow::StatusUpdateReset()
{
	MyStatus->Reset();
	MyStatus->Update(MyStatus->MaxValue());
}


void StatusWindow::MessageReceived(BMessage* msg)
{
	switch (msg->what) {
		case 'ClWi': {
				Quit();
			}
			break;
		case 'More': {
				if (!fullview) {
					fullview = !fullview;
					ResizeBy(0.0, 150.0);
					More->SetTitle("Less..");
				} else {
					fullview = !fullview;
					ResizeBy(0.0, -150.0);
					More->SetTitle("More..");
				}
			}
			break;
	}
}


void StatusWindow::Ready()
{
	char temp[1024];
	sprintf(temp, "Done!");
	StatusSetColor(blue);
	StatusUpdateReset();
	StatusSetText(temp);
	Close->SetEnabled(true);
	TRACK_FIN = 1000;
	SView->Invalidate();
}


void StatusWindow::SetAngles(float ang[100], int tracks)
{
	SView->SetAngles(ang, tracks);
	SView->Invalidate();
}
