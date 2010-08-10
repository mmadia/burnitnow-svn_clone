/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include "AudioView.h"
extern char PAD[10]; //-pad (audio)
extern char NOFIX[10]; // -nofix (audio)
extern char PREEMP[10]; // -preemp (audio)
extern char SWAB[10]; // -swab
AudioView::AudioView (BRect size) : BView(size,"AudioView",B_FOLLOW_NONE,B_WILL_DRAW) {
	BRect r,r2;
	SetViewColor(216,216,216,0);

	AdvLabel = new IconLabel(BRect(0,0,19+be_bold_font->StringWidth(" Audio Options (see help before change options)"),19)," Audio Options (see help before change options)","BMP:AUDIOOPT");
	AdvLabel->SetViewColor(grey);
	r = Bounds();
	r.InsetBy(5.0,5.0);
	r.bottom = 80;
	AdvOptions = new BBox(r,"AdvOptions",B_FOLLOW_NONE,B_WILL_DRAW|B_FRAME_EVENTS,B_PLAIN_BORDER);
	AdvOptions->SetLabel(AdvLabel);
	AddChild(AdvOptions);


	r = AdvOptions->Bounds();
	r.InsetBy(5.0,5.0);
	r.top += 10;
	r.right = 120;
	r.bottom = 25;
	Pad = new BCheckBox(r,"pad","Padding",new BMessage(AUDIO_PAD));
	if(!strncmp(PAD,"-pad",4)) {
		Pad->SetValue(B_CONTROL_ON);
	}
	AdvOptions->AddChild(Pad);

	r.bottom +=15;
	r.top += 15;
	r.right = 120;
	Nofix = new BCheckBox(r,"nofix","No fixate",new BMessage(AUDIO_NOFIX));
	if(!strncmp(NOFIX,"-nofix",6)) {
		Nofix->SetValue(B_CONTROL_ON);
	}
	AdvOptions->AddChild(Nofix);

	r.bottom +=15;
	r.top += 15;
	Preemp = new BCheckBox(r,"preemp","Preemp",new BMessage(AUDIO_PREEMP));
	if(!strncmp(PREEMP,"-preemp",7)) {
		Preemp->SetValue(B_CONTROL_ON);
	}
	AdvOptions->AddChild(Preemp);

	r = AdvOptions->Bounds();
	r.InsetBy(5.0,5.0);
	r.top += 10;
	r.left = 125;
	r.bottom = 25;

	Swab = new BCheckBox(r,"swab","Swab",new BMessage(AUDIO_SWAB));
	if(!strncmp(SWAB,"-swab",5)) {
		Swab->SetValue(B_CONTROL_ON);
	}
	AdvOptions->AddChild(Swab);



	//Info box
	r = Bounds();
	r.InsetBy(5.0,5.0);
	r.top = 85;
	Info = new BBox(r,"Info",B_FOLLOW_NONE,B_WILL_DRAW|B_FRAME_EVENTS,B_PLAIN_BORDER);
	Info->SetLabel("Audio Info");
	AddChild(Info);
	
	r = Info->Bounds();
	r.InsetBy(5.0,5.0);
	r.top += 10;
	r.right -= B_V_SCROLL_BAR_WIDTH;
	r2 = r;
	r2.InsetBy(2.0,2.0);
	InfoText = new BTextView(r,"InfoText",r2,B_FOLLOW_NONE,B_WILL_DRAW);
	InfoText->MakeEditable(false);
	InfoText->SetStylable(true);
	InfoScroll=new BScrollView("Infoscroll",InfoText,B_FOLLOW_NONE,0,false,true,B_FANCY_BORDER);
	Info->AddChild(InfoScroll);
	InfoText->SetFontAndColor(be_fixed_font,B_FONT_ALL,&darkblue);
	InfoText->Insert("Here will be some AudioInfo in the future like what codecs there are on the system.");

}
AudioView::~AudioView() {
}
