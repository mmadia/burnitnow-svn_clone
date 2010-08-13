/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "DataView.h"

#include "const.h"
#include "IconLabel.h"


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
	r.bottom = 90; // was 85
	fFileSystemLabel = new IconLabel(BRect(0, 0, 19 + be_bold_font->StringWidth(" Filesystem"), 19), " Filesystem", "BMP:FS");
	fFileSystemLabel->SetViewColor(grey);
	fFileSystemBox = new BBox(r, "fFileSystemBox", B_FOLLOW_NONE, B_WILL_DRAW | B_FRAME_EVENTS, B_PLAIN_BORDER);
	fFileSystemBox->SetLabel(fFileSystemLabel);
	AddChild(fFileSystemBox);
	r = fFileSystemBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 10;
	r.right = 220; // was 170
	r.bottom = 25;
	fISO9660Radio = new BRadioButton(r, "ISO-9660", "ISO-9660", new BMessage(DATA_ISO9660));
	if (!strcmp(DATA_STRING, " ") && IMAGE_TYPE == 0)
		fISO9660Radio->SetValue(B_CONTROL_ON);
	fFileSystemBox->AddChild(fISO9660Radio);


	r.bottom += 15;
	r.top += 15;
	fWindowsRadio = new BRadioButton(r, "ISO-9660 with long filenames", "ISO-9660 with long filenames", new BMessage(DATA_WINDOWS));
	if (!strcmp(DATA_STRING, "-D -l") && IMAGE_TYPE == 0)
		fWindowsRadio->SetValue(B_CONTROL_ON);
	fFileSystemBox->AddChild(fWindowsRadio);

	r.bottom += 15;
	r.top += 15;
	fJolietRadio = new BRadioButton(r, "fJolietRadio", "Windows (Joliet)", new BMessage(DATA_JOLIET));
	if (!strcmp(DATA_STRING, "-l -D -J") && IMAGE_TYPE == 0)
		fJolietRadio->SetValue(B_CONTROL_ON);
	fFileSystemBox->AddChild(fJolietRadio);

	r.bottom += 15;
	r.top += 15;
	fBeOSRadio = new BRadioButton(r, "Haiku", "Haiku (bfs)", new BMessage(DATA_BFS));
	if (IMAGE_TYPE == 1)
		fBeOSRadio->SetValue(B_CONTROL_ON);

	fFileSystemBox->AddChild(fBeOSRadio);


	r = fFileSystemBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 10;
	r.bottom = 25;
	r.left = 230; // was 175
	fRockRadio = new BRadioButton(r, "RockRidge (UNIX Multiuser)", "RockRidge (UNIX Singeluser)", new BMessage(DATA_ROCK));
	if (!strcmp(DATA_STRING, "-l -L -r") && IMAGE_TYPE == 0)
		fRockRadio->SetValue(B_CONTROL_ON);
	fFileSystemBox->AddChild(fRockRadio);

	r.bottom += 15;
	r.top += 15;
	fRealRockRadio = new BRadioButton(r, "Real RockRidge (UNIX Multiuser)", "Real RockRidge (UNIX Multiuser)", new BMessage(DATA_REALROCK));
	if (!strcmp(DATA_STRING, "-l -L -R") && IMAGE_TYPE == 0)
		fRealRockRadio->SetValue(B_CONTROL_ON);
	fFileSystemBox->AddChild(fRealRockRadio);

	r.bottom += 15;
	r.top += 15;
	fMacRadio = new BRadioButton(r, "fMacRadio", "Mac (hfs)", new BMessage(DATA_HFS));
	if (!strcmp(DATA_STRING, "-hfs") && IMAGE_TYPE == 0)
		fMacRadio->SetValue(B_CONTROL_ON);
	fFileSystemBox->AddChild(fMacRadio);

	r.bottom += 15;
	r.top += 15;
	fOwnRadio = new BRadioButton(r, "own", "Own (choose mkisofs options)", new BMessage(DATA_HFS));
	//if(!strcmp(DATA_STRING,"-hfs"))
	//	fMacRadio->SetValue(B_CONTROL_ON);
	fOwnRadio->SetEnabled(false);
	fFileSystemBox->AddChild(fOwnRadio);

	r = Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = 95;
	r.right = r.right - 160; // was 130

	fBootLabel = new IconLabel(BRect(0, 0, 19 + be_bold_font->StringWidth(" El Torito Bootable CD"), 19), " El Torito Bootable CD", "BMP:BOOTABLE");
	fBootLabel->SetViewColor(grey);
	fBootBox = new BBox(r, "BootBox", B_FOLLOW_NONE, B_WILL_DRAW | B_FRAME_EVENTS, B_PLAIN_BORDER);
	fBootBox->SetLabel(fBootLabel);
	AddChild(fBootBox);

	r = fBootBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 10;
	r.bottom = r.top + 15;
	fBootableCDCheckBox = new BCheckBox(r, "fBootableCDCheckBox", "Enable BootableCD", new BMessage(BOOT_CHECKED));
	if (BOOTABLE)
		fBootableCDCheckBox->SetValue(B_CONTROL_ON);
	if (!VRCD)
		fBootableCDCheckBox->SetEnabled(false);

	fBootBox->AddChild(fBootableCDCheckBox);

	r = fBootBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 35;
	fChooseBootImageButton = new BButton(r, "fChooseBootImageButton", "Choose boot image", new BMessage(BOOT_FILE_PANEL));
	if (!BOOTABLE || !VRCD)
		fChooseBootImageButton->SetEnabled(false);
	fBootBox->AddChild(fChooseBootImageButton);

	r = Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = 135;
	r.left = r.right - 150; // was 120
	r.bottom = 165;

	fChangeVolNameButton = new BButton(r, "Change Volume Name", "Change Volume Name", new BMessage(CHANGE_VOL_NAME));
	AddChild(fChangeVolNameButton);

	fFilePanel = new BFilePanel(B_OPEN_PANEL);
	fFilePanel->SetMessage(new BMessage(BOOT_CHANGE_IMAGE_NAME));

}


DataView::~DataView()
{
}
