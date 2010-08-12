/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "BootView.h"


extern char DAO[10];


BootView::BootView(BRect size)
	:
	BView(size, "BootView", B_FOLLOW_NONE, B_WILL_DRAW)
{
	BRect r;
	SetViewColor(216, 216, 216, 0);
	r = Bounds();
	r.InsetBy(5.0, 5.0);
	r.bottom = r.top + 20;
	Recorders = new BMenu("Select");
	Recorders->SetLabelFromMarked(true);
	RecordPop = new BMenuField(r, "recorder", "Devices:", Recorders);
	RecordPop->SetDivider(be_plain_font->StringWidth("Devices:  "));
	AddChild(RecordPop);

	MiscLabel = new IconLabel(BRect(0, 0, 19 + be_bold_font->StringWidth(" Misc Options (see help before change options)"), 19), " Misc Options (see help before change options)", "BMP:MISCOPT");
	MiscLabel->SetViewColor(grey);
	r = Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = r.top + 25;
	MiscOpt = new BBox(r, "MiscOptions", B_FOLLOW_NONE, B_WILL_DRAW | B_FRAME_EVENTS, B_PLAIN_BORDER);
	MiscOpt->SetLabel(MiscLabel);
	AddChild(MiscOpt);

	r = MiscOpt->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 10;
	r.right = 170; // was 120
	r.bottom = 40;
	DAOOption = new BCheckBox(r, "DAO", "DAO (Disc At Once)", new BMessage(MISC_DAO));
	if (!strncmp(DAO, "-dao", 4)) {
		DAOOption->SetValue(B_CONTROL_ON);
	}
	MiscOpt->AddChild(DAOOption);
}


BootView::~BootView()
{
}
