/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "LogView.h"

#include "const.h"
#include "IconLabel.h"

#include <ScrollView.h>


LogView::LogView(BRect size)
	:
	BView(size, "LogView", B_FOLLOW_NONE, B_WILL_DRAW)
{
	BRect r, r2;
	SetViewColor(216, 216, 216, 0);
	// fLogBox
	fLogLabel = new IconLabel(BRect(0, 0, 19 + be_bold_font->StringWidth(""), 19), "", "BMP:LOG");
	fLogLabel->SetViewColor(grey);
	r = Bounds();
	//r.InsetBy(5.0,5.0);
	fLogBox = new BBox(r, "Boxlog", B_FOLLOW_NONE, B_WILL_DRAW | B_FRAME_EVENTS, B_PLAIN_BORDER);
	fLogBox->SetLabel(fLogLabel);
	AddChild(fLogBox);

	// This will be inside the fLogBox
	r = fLogBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.right -= B_V_SCROLL_BAR_WIDTH;
	r.top += 15;
	r2 = r;
	r2.InsetBy(2.0, 2.0);
	fLogTextView = new BTextView(r, "fLogTextView", r2, B_FOLLOW_NONE, B_WILL_DRAW);
	fLogTextView->MakeEditable(false);
	fLogTextView->SetStylable(true);
	fLogScrollView = new BScrollView("logscroll", fLogTextView, B_FOLLOW_NONE, 0, false, true, B_FANCY_BORDER);
	fLogBox->AddChild(fLogScrollView);
}


LogView::~LogView()
{
}
