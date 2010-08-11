/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "DataView.h"


extern char DATA_STRING[100];
extern bool BOOTABLE;
extern bool VRCD;
extern int16 IMAGE_TYPE;


DataView::DataView(BRect size)
	:
	BView(size, "DataView", B_FOLLOW_NONE, B_WILL_DRAW)
{
	SetViewColor(216, 216, 216, 0);
	BRect r;
	r = Bounds();
	r.InsetBy(5.0, 5.0);
	r.bottom = 85;
	FSLabel = new IconLabel(BRect(0, 0, 19 + be_bold_font->StringWidth(" Filesystem"), 19), " Filesystem", "BMP:FS");
	FSLabel->SetViewColor(grey);
	FSBox = new BBox(r, "FSBox", B_FOLLOW_NONE, B_WILL_DRAW | B_FRAME_EVENTS, B_PLAIN_BORDER);
	FSBox->SetLabel(FSLabel);
	AddChild(FSBox);
	r = FSBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 10;
	r.right = 170;
	r.bottom = 25;
	ISO9660 = new BRadioButton(r, "ISO-9660", "ISO-9660", new BMessage(DATA_ISO9660));
	if (!strcmp(DATA_STRING, " ") && IMAGE_TYPE == 0)
		ISO9660->SetValue(B_CONTROL_ON);
	FSBox->AddChild(ISO9660);


	r.bottom += 15;
	r.top += 15;
	Windows = new BRadioButton(r, "ISO-9660 with long filenames", "ISO-9660 with long filenames", new BMessage(DATA_WINDOWS));
	if (!strcmp(DATA_STRING, "-D -l") && IMAGE_TYPE == 0)
		Windows->SetValue(B_CONTROL_ON);
	FSBox->AddChild(Windows);

	r.bottom += 15;
	r.top += 15;
	Joliet = new BRadioButton(r, "Joliet", "Windows (Joliet)", new BMessage(DATA_JOLIET));
	if (!strcmp(DATA_STRING, "-l -D -J") && IMAGE_TYPE == 0)
		Joliet->SetValue(B_CONTROL_ON);
	FSBox->AddChild(Joliet);

	r.bottom += 15;
	r.top += 15;
	BeOS = new BRadioButton(r, "BeOS", "BeOS (bfs)", new BMessage(DATA_BFS));
	if (IMAGE_TYPE == 1)
		BeOS->SetValue(B_CONTROL_ON);

	FSBox->AddChild(BeOS);


	r = FSBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 10;
	r.bottom = 25;
	r.left = 175;
	Rock = new BRadioButton(r, "RockRidge (UNIX Multiuser)", "RockRidge (UNIX Singeluser)", new BMessage(DATA_ROCK));
	if (!strcmp(DATA_STRING, "-l -L -r") && IMAGE_TYPE == 0)
		Rock->SetValue(B_CONTROL_ON);
	FSBox->AddChild(Rock);

	r.bottom += 15;
	r.top += 15;
	RealRock = new BRadioButton(r, "Real RockRidge (UNIX Multiuser)", "Real RockRidge (UNIX Multiuser)", new BMessage(DATA_REALROCK));
	if (!strcmp(DATA_STRING, "-l -L -R") && IMAGE_TYPE == 0)
		RealRock->SetValue(B_CONTROL_ON);
	FSBox->AddChild(RealRock);

	r.bottom += 15;
	r.top += 15;
	Mac = new BRadioButton(r, "Mac", "Mac (hfs)", new BMessage(DATA_HFS));
	if (!strcmp(DATA_STRING, "-hfs") && IMAGE_TYPE == 0)
		Mac->SetValue(B_CONTROL_ON);
	FSBox->AddChild(Mac);

	r.bottom += 15;
	r.top += 15;
	Own = new BRadioButton(r, "own", "Own (choose mkisofs options)", new BMessage(DATA_HFS));
	//if(!strcmp(DATA_STRING,"-hfs"))
	//	Mac->SetValue(B_CONTROL_ON);
	Own->SetEnabled(false);
	FSBox->AddChild(Own);

	r = Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = 95;
	r.right = r.right - 130;

	BootLabel = new IconLabel(BRect(0, 0, 19 + be_bold_font->StringWidth(" El Torito Bootable CD"), 19), " El Torito Bootable CD", "BMP:BOOTABLE");
	BootLabel->SetViewColor(grey);
	BootBox = new BBox(r, "BootBox", B_FOLLOW_NONE, B_WILL_DRAW | B_FRAME_EVENTS, B_PLAIN_BORDER);
	BootBox->SetLabel(BootLabel);
	AddChild(BootBox);

	r = BootBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 10;
	r.bottom = r.top + 15;
	BootableCD = new BCheckBox(r, "Enable BootableCD", "Enable BootableCD", new BMessage(BOOT_CHECKED));
	if (BOOTABLE)
		BootableCD->SetValue(B_CONTROL_ON);
	if (!VRCD)
		BootableCD->SetEnabled(false);

	BootBox->AddChild(BootableCD);

	r = BootBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 35;
	ChooseBootImage = new jpButton(r, "ChooseBootImage", "Choose boot image", new BMessage(BOOT_FILE_PANEL));
	if (!BOOTABLE || !VRCD)
		ChooseBootImage->SetEnabled(false);
	BootBox->AddChild(ChooseBootImage);

	r = Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = 135;
	r.left = r.right - 120;
	r.bottom = 165;

	ChangeVolName = new jpButton(r, "Change Volume Name", "Change Volume Name", new BMessage(CHANGE_VOL_NAME));
	AddChild(ChangeVolName);

	FilePanel = new BFilePanel(B_OPEN_PANEL);
	FilePanel->SetMessage(new BMessage(BOOT_CHANGE_IMAGE_NAME));

}


DataView::~DataView()
{
}
