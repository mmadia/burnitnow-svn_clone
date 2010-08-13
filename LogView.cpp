/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "LogView.h"

#include "const.h"
#include "IconLabel.h"

#include <Box.h>
#include <ScrollView.h>


LogView::LogView(BRect size)
	:
	BView(size, "LogView", B_FOLLOW_NONE, B_WILL_DRAW)
{
	BRect r, r2;
	SetViewColor(216, 216, 216, 0);
	// fLogBox
	IconLabel* logLabel = new IconLabel(BRect(0, 0, 19 + be_bold_font->StringWidth(""), 19), "", "BMP:LOG");
	logLabel->SetViewColor(grey);
	r = Bounds();
	//r.InsetBy(5.0,5.0);
	BBox* logBox = new BBox(r, "logBox", B_FOLLOW_NONE, B_WILL_DRAW | B_FRAME_EVENTS, B_PLAIN_BORDER);
	logBox->SetLabel(logLabel);
	AddChild(logBox);

	// This will be inside the fLogBox
	r = logBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.right -= B_V_SCROLL_BAR_WIDTH;
	r.top += 15;
	r2 = r;
	r2.InsetBy(2.0, 2.0);
	fLogTextView = new BTextView(r, "fLogTextView", r2, B_FOLLOW_NONE, B_WILL_DRAW);
	fLogTextView->MakeEditable(false);
	fLogTextView->SetStylable(true);
	BScrollView* logScrollView = new BScrollView("logScrollView", fLogTextView, B_FOLLOW_NONE, 0, false, true, B_FANCY_BORDER);
	logBox->AddChild(logScrollView);
}


LogView::~LogView()
{
}
