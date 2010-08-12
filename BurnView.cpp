/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "BurnView.h"


extern int16 BURN_SPD;
extern int16 BURN_TYPE;
extern char MULTISESSION[10];
extern char DUMMYMODE[10];
extern char EJECT[10];
extern bool ONTHEFLY;


BurnView::BurnView(BRect size)
	:
	BView(size, "BurnView", B_FOLLOW_NONE, B_WILL_DRAW)
{
	BRect r, r2;
	SetViewColor(216, 216, 216, 0);
	char temp_char[100];
	// BurnBox
	BurnLabel = new IconLabel(BRect(0, 0, 19 + be_bold_font->StringWidth(" Burn"), 19), " Burn", "BMP:BURN");
	BurnLabel->SetViewColor(grey);
	r = Bounds();
	r.InsetBy(5.0, 5.0);
	BurnBox = new BBox(r, "BurnBox", B_FOLLOW_NONE, B_WILL_DRAW | B_FRAME_EVENTS, B_PLAIN_BORDER);
	BurnBox->SetLabel(BurnLabel);
	AddChild(BurnBox);
	r = BurnBox->Bounds();
	r.left = 140;
	r.right = 204;
	r.top = 50;
	r.bottom = 114;
	bg = new IconLabel(r, "", "BMP:BACKGROUND");
	bg->SetViewColor(grey);
	BurnBox->AddChild(bg);
	// RadioButtons
	r = BurnBox->Bounds();
	r2 = r;
	r.InsetBy(5.0, 5.0);
	r.left += 25;
	r.top += 15;
	r.bottom = 30;
	r.right = 125;
	r2.InsetBy(5.0, 5.0);
	r2.left += 6;
	r2.right = 27;
	r2.top += 10;
	r2.bottom = 30;
	DataCDLabel = new IconLabel(r2, "", "BMP:DATACD");
	DataCDLabel->SetViewColor(grey);
	BurnBox->AddChild(DataCDLabel);

	r2.top += 22;
	r2.bottom += 22;
	AudioCDLabel = new IconLabel(r2, "", "BMP:AUDIOCD");
	AudioCDLabel->SetViewColor(grey);
	BurnBox->AddChild(AudioCDLabel);

	r2.top += 22;
	r2.bottom += 22;
	MixCDLabel = new IconLabel(r2, "", "BMP:MIXCD");
	MixCDLabel->SetViewColor(grey);
	BurnBox->AddChild(MixCDLabel);

	RadioDataCD = new BRadioButton(r, "DataCD", "DataCD", new BMessage(BURN_DATA_CD));
	if (BURN_TYPE == 0)
		RadioDataCD->SetValue(B_CONTROL_ON);
	BurnBox->AddChild(RadioDataCD);

	r.top += 20;
	r.bottom += 20;
	RadioAudioCD = new BRadioButton(r, "AudioCD", "AudioCD", new BMessage(BURN_AUDIO_CD));
	if (BURN_TYPE == 1)
		RadioAudioCD->SetValue(B_CONTROL_ON);
	BurnBox->AddChild(RadioAudioCD);

	r.top += 20;
	r.bottom += 20;
	RadioMixCD = new BRadioButton(r, "MixCD", "MixCD", new BMessage(BURN_MIX_CD));
	if (BURN_TYPE == 2)
		RadioMixCD->SetValue(B_CONTROL_ON);
	BurnBox->AddChild(RadioMixCD);
	// CheckBoxes
	r.top += 35;
	r.bottom += 35;
	r.left = 13;
	r.right = 125;
	Multi = new BCheckBox(r, "MultiSession", "MultiSession", new BMessage(BURN_MULTI));
	if (!strncmp(MULTISESSION, "-multi", 6)) {
		Multi->SetValue(B_CONTROL_ON);
	}
	BurnBox->AddChild(Multi);

	r.top += 15;
	r.bottom += 15;
	OnTheFly = new BCheckBox(r, "OnTheFly", "On the fly", new BMessage(BURN_ONTHEFLY));
	if (ONTHEFLY == true) {
		OnTheFly->SetValue(B_CONTROL_ON);
		Multi->SetEnabled(false);
	}
	BurnBox->AddChild(OnTheFly);

	r.top += 15;
	r.bottom += 15;
	DummyMode = new BCheckBox(r, "DummyMode", "Dummy mode", new BMessage(BURN_DUMMY_MODE));
	if (!strncmp(DUMMYMODE, "-dummy", 6)) {
		DummyMode->SetValue(B_CONTROL_ON);
	}
	BurnBox->AddChild(DummyMode);

	r.top += 15;
	r.bottom += 15;
	Eject = new BCheckBox(r, "Eject", "Eject", new BMessage(BURN_EJECT));
	if (!strncmp(EJECT, "-eject", 6)) {
		Eject->SetValue(B_CONTROL_ON);
	}

	BurnBox->AddChild(Eject);


	// BurnButton
	r = BurnBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = 120;
	r.bottom -= 5;
	r.left = 370; // was 260
	r.right = 460; // was 350
	BurnButton = new jpButton(r, "BurnButton", "Burn!", new BMessage(MAKE_IMAGE));
	BurnBox->AddChild(BurnButton);

	r = BurnBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = 120;
	r.bottom -= 5;
	r.left = 270; //was 160
	r.right = 360; //was 250
	MakeImage = new jpButton(r, "MakeImage", "Make Image", new BMessage(MAKE_AND_SAVE_IMAGE));
	BurnBox->AddChild(MakeImage);
	// BurningSpeed
	r = BurnBox->Bounds();
	r.InsetBy(5.0, 5.0);
	r.left = 225;
	r.right = 350;
	r.top += 10;
	sprintf(temp_char, "Burning Speed [%dx]", BURN_SPD);
	Speed = new BSlider(r, "Speed", temp_char, new BMessage(SPEED_CHANGE), 0, 5, B_BLOCK_THUMB, B_FOLLOW_NONE);
	Speed->SetHashMarks(B_HASH_MARKS_BOTTOM);
	Speed->SetHashMarkCount(6);
	BurnBox->AddChild(Speed);
	Speed->SetValue((int32)(BURN_SPD / 2) - 1);


}


BurnView::~BurnView()
{
	delete bg;
}


void BurnView::SetButton(bool what)
{
	BurnButton->SetEnabled(what);
	MakeImage->SetEnabled(what);
}


void BurnView::SetLabel(char* label)
{
	BurnButton->SetLabel(label);
}
