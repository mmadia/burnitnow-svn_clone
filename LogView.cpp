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
	// BoxLog
	LogLabel = new IconLabel(BRect(0, 0, 19 + be_bold_font->StringWidth(""), 19), "", "BMP:LOG");
	LogLabel->SetViewColor(grey);
	r = Bounds();
	//r.InsetBy(5.0,5.0);
	BoxLog = new BBox(r, "Boxlog", B_FOLLOW_NONE, B_WILL_DRAW | B_FRAME_EVENTS, B_PLAIN_BORDER);
	BoxLog->SetLabel(LogLabel);
	AddChild(BoxLog);

	// This will be inside the BoxLog
	r = BoxLog->Bounds();
	r.InsetBy(5.0, 5.0);
	r.right -= B_V_SCROLL_BAR_WIDTH;
	r.top += 15;
	r2 = r;
	r2.InsetBy(2.0, 2.0);
	LogText = new BTextView(r, "LogText", r2, B_FOLLOW_NONE, B_WILL_DRAW);
	LogText->MakeEditable(false);
	LogText->SetStylable(true);
	LogScroll = new BScrollView("logscroll", LogText, B_FOLLOW_NONE, 0, false, true, B_FANCY_BORDER);
	BoxLog->AddChild(LogScroll);
}


LogView::~LogView()
{
}
