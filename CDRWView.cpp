/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "CDRWView.h"

#include "const.h"
#include "IconLabel.h"

#include <stdio.h>

#include <Box.h>
#include <Button.h>
#include <MenuField.h>
#include <MenuItem.h>


extern int16 BLANK_SPD;


CDRWView::CDRWView(BRect size)
	:
	BView(size, "CDRWView", B_FOLLOW_NONE, B_WILL_DRAW)
{
	BRect r;
	char temp_char[100];
	SetViewColor(216, 216, 216, 0);
	// CDRW BOX
	IconLabel* CDRWLabel = new IconLabel(BRect(0, 0, 19 + be_bold_font->StringWidth(""), 19), "", "BMP:CDRW");
	CDRWLabel->SetViewColor(grey);
	r = Bounds();
	r.InsetBy(5.0, 5.0);
	r.bottom = r.top + 70;
	BBox* CDRWBox = new BBox(r, "CDRWBox", B_FOLLOW_NONE, B_WILL_DRAW | B_FRAME_EVENTS, B_PLAIN_BORDER);
	CDRWBox->SetLabel(CDRWLabel);
	AddChild(CDRWBox);

	// BlankMenu (CDRWBOX)
	r = CDRWBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 25;
	fBlankMenu = new BMenu("Select");
	fBlankMenu->SetLabelFromMarked(true);
	BMenuField* blankMenuField = new BMenuField(r, "blank", "Blank:", fBlankMenu);
	blankMenuField->SetDivider(be_plain_font->StringWidth("Blank:  "));
	CDRWBox->AddChild(blankMenuField);
	fBlankMenu->AddItem(new BMenuItem("Full", new BMessage(BLANK_FULL)));
	fBlankMenu->AddItem(new BMenuItem("Fast", new BMessage(BLANK_FAST)));
	fBlankMenu->AddItem(new BMenuItem("Session", new BMessage(BLANK_SESSION)));
	fBlankMenu->AddItem(new BMenuItem("Track", new BMessage(BLANK_TRACK)));
	fBlankMenu->AddItem(new BMenuItem("Track tail", new BMessage(BLANK_TRACK_TAIL)));
	fBlankMenu->AddItem(new BMenuItem("Unreserve", new BMessage(BLANK_UNRES)));
	fBlankMenu->AddItem(new BMenuItem("Unclose", new BMessage(BLANK_UNCLOSE)));


	// BlankSpeed
	r = CDRWBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 15;
	r.left = 155; // was 125
	r.right = 250;
	sprintf(temp_char, "Blank Speed [%dx]", BLANK_SPD);
	fBlankSpeedSlider = new BSlider(r, "BlankSpeed", temp_char, new BMessage(BLANK_SPEED_CHANGE), 0, 5, B_BLOCK_THUMB, B_FOLLOW_NONE);
	fBlankSpeedSlider->SetHashMarks(B_HASH_MARKS_BOTTOM);
	fBlankSpeedSlider->SetHashMarkCount(6);
	fBlankSpeedSlider->SetValue((int32)(BLANK_SPD / 2) - 1);
	CDRWBox->AddChild(fBlankSpeedSlider);

	// BlankButton
	r = CDRWBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 10;
	r.left = 300;
	r.right = 365;
	fBlankButton = new BButton(r, "BlankButton", "Blank!", new BMessage(BLANK_IT_NOW));
	CDRWBox->AddChild(fBlankButton);

}


CDRWView::~CDRWView()
{
}
