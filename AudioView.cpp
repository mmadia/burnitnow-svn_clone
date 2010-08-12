/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "AudioView.h"

#include "const.h"
#include "IconLabel.h"

#include <string.h>

#include <ScrollView.h>


extern char PAD[10]; //-pad (audio)
extern char NOFIX[10]; // -nofix (audio)
extern char PREEMP[10]; // -preemp (audio)
extern char SWAB[10]; // -swab


AudioView::AudioView(BRect size)
	:
	BView(size, "AudioView", B_FOLLOW_NONE, B_WILL_DRAW)
{
	BRect r, r2;
	SetViewColor(216, 216, 216, 0);

	fAdvLabel = new IconLabel(BRect(0, 0, 19 + be_bold_font->StringWidth(" Audio Options (see help before change options)"), 19), " Audio Options (see help before change options)", "BMP:AUDIOOPT");
	fAdvLabel->SetViewColor(grey);
	r = Bounds();
	r.InsetBy(5.0, 5.0);
	r.bottom = 80;
	fAdvOptionsBox = new BBox(r, "fAdvOptionsBox", B_FOLLOW_NONE, B_WILL_DRAW | B_FRAME_EVENTS, B_PLAIN_BORDER);
	fAdvOptionsBox->SetLabel(fAdvLabel);
	AddChild(fAdvOptionsBox);


	r = fAdvOptionsBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 10;
	r.right = 120;
	r.bottom = 25;
	fPadCheckBox = new BCheckBox(r, "pad", "Padding", new BMessage(AUDIO_PAD));
	if (!strncmp(PAD, "-pad", 4)) {
		fPadCheckBox->SetValue(B_CONTROL_ON);
	}
	fAdvOptionsBox->AddChild(fPadCheckBox);

	r.bottom += 15;
	r.top += 15;
	r.right = 120;
	fNoFixCheckBox = new BCheckBox(r, "nofix", "No fixate", new BMessage(AUDIO_NOFIX));
	if (!strncmp(NOFIX, "-nofix", 6)) {
		fNoFixCheckBox->SetValue(B_CONTROL_ON);
	}
	fAdvOptionsBox->AddChild(fNoFixCheckBox);

	r.bottom += 15;
	r.top += 15;
	fPreEmpCheckBox = new BCheckBox(r, "preemp", "Preemp", new BMessage(AUDIO_PREEMP));
	if (!strncmp(PREEMP, "-preemp", 7)) {
		fPreEmpCheckBox->SetValue(B_CONTROL_ON);
	}
	fAdvOptionsBox->AddChild(fPreEmpCheckBox);

	r = fAdvOptionsBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 10;
	r.left = 125;
	r.bottom = 25;

	fSwabCheckBox = new BCheckBox(r, "swab", "Swab", new BMessage(AUDIO_SWAB));
	if (!strncmp(SWAB, "-swab", 5)) {
		fSwabCheckBox->SetValue(B_CONTROL_ON);
	}
	fAdvOptionsBox->AddChild(fSwabCheckBox);



	// Info box
	r = Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = 85;
	fInfoBox = new BBox(r, "Info", B_FOLLOW_NONE, B_WILL_DRAW | B_FRAME_EVENTS, B_PLAIN_BORDER);
	fInfoBox->SetLabel("Audio Info");
	AddChild(fInfoBox);

	r = fInfoBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top += 10;
	r.right -= B_V_SCROLL_BAR_WIDTH;
	r2 = r;
	r2.InsetBy(2.0, 2.0);
	fInfoTextView = new BTextView(r, "fInfoTextView", r2, B_FOLLOW_NONE, B_WILL_DRAW);
	fInfoTextView->MakeEditable(false);
	fInfoTextView->SetStylable(true);
	fInfoScrollView = new BScrollView("Infoscroll", fInfoTextView, B_FOLLOW_NONE, 0, false, true, B_FANCY_BORDER);
	fInfoBox->AddChild(fInfoScrollView);
	fInfoTextView->SetFontAndColor(be_fixed_font, B_FONT_ALL, &darkblue);
	fInfoTextView->Insert("Here will be some AudioInfo in the future like what codecs there are on the system.");

}


AudioView::~AudioView()
{
}
