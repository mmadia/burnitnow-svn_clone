/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "PrefsView.h"

#include "const.h"
#include "IconLabel.h"

#include <string.h>


extern char DAO[10];
extern char BURNPROOF[30];


PrefsView::PrefsView(BRect size)
	:
	BView(size, "PrefsView", B_FOLLOW_NONE, B_WILL_DRAW)
{
	BRect r;
	SetViewColor(216, 216, 216, 0);
	r = Bounds();
	r.InsetBy(5.0, 5.0);
	r.bottom = r.top + 20;
	fRecordersMenu = new BMenu("Select");
	fRecordersMenu->SetLabelFromMarked(true);
	fRecordMenuField = new BMenuField(r, "recorder", "Devices:", fRecordersMenu);
	fRecordMenuField->SetDivider(be_plain_font->StringWidth("Devices:  "));
	AddChild(fRecordMenuField);

	fMiscLabel = new IconLabel(BRect(0, 0, 19 + be_bold_font->StringWidth(" Misc Options (see help before change options)"), 19), " Misc Options (see help before change options)", "BMP:MISCOPT");
	fMiscLabel->SetViewColor(grey);
	r = Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = r.top + 25;
	fMiscOptBox = new BBox(r, "fMiscOptBoxions", B_FOLLOW_NONE, B_WILL_DRAW | B_FRAME_EVENTS, B_PLAIN_BORDER);
	fMiscOptBox->SetLabel(fMiscLabel);
	AddChild(fMiscOptBox);

	r = fMiscOptBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 10;
	r.right = 175; // was 125
	r.bottom = 40;
	fDAOCheckBox = new BCheckBox(r, "DAO", "DAO (Disc At Once)", new BMessage(MISC_DAO));
	if (!strncmp(DAO, "-dao", 4)) {
		fDAOCheckBox->SetValue(B_CONTROL_ON);
	}
	fMiscOptBox->AddChild(fDAOCheckBox);

	r.top += 20;
	r.bottom += 20;
	fBurnProofCheckBox = new BCheckBox(r, "burnproof", "BurnProof (read help)", new BMessage(MISC_BURNPROOF));
	if (!strncmp(BURNPROOF, "driveropts = burnproof", 22)) {
		fBurnProofCheckBox->SetValue(B_CONTROL_ON);
	}
	fMiscOptBox->AddChild(fBurnProofCheckBox);

}


PrefsView::~PrefsView()
{
}
