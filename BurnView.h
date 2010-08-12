/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _BURNVIEW_H_
#define _BURNVIEW_H_


#include <Box.h>
#include <CheckBox.h>
#include <RadioButton.h>
#include <Slider.h>


// Forward declaration of IconLabel and jpButton classes
class IconLabel;
class jpButton;


class BurnView : public BView {
public:
	BurnView(BRect size);
	~BurnView();
	void SetButton(bool);
	void SetLabel(char*);
	// CDRW public

	// Burn public
	BSlider* BurnSpeed;
	jpButton* BurnButton, *MakeImage;
	BCheckBox* Multi, *OnTheFly, *Eject, *DummyMode;
	BSlider* Speed;

private:
	// BurnBox
	BBox* BurnBox;
	IconLabel* BurnLabel, *DataCDLabel, *AudioCDLabel, *MixCDLabel, *bg;
	BRadioButton* RadioAudioCD, *RadioDataCD, *RadioMixCD;
};


#endif	// _BURNVIEW_H_
