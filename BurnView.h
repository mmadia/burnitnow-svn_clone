/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _BURNVIEW_H_
#define _BURNVIEW_H_


#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <RadioButton.h>
#include <Slider.h>


// Forward declaration of IconLabel
class IconLabel;


class BurnView : public BView {
public:
	BurnView(BRect size);
	~BurnView();
	void SetButton(bool);
	void SetLabel(char*);
	// CDRW public

	// Burn public
	BSlider* fBurnSpeedSlider;
	BButton* fBurnButton, *fMakeImageButton;
	BCheckBox* fMultiCheckBox, *fOnTheFlyCheckBox, *fEjectCheckBox, *fDummyModeCheckBox;
	BSlider* fSpeedSlider;

private:
	// BurnBox
	BBox* fBurnBox;
	IconLabel* fBurnLabel, *fDataCDLabel, *fAudioCDLabel, *fMixCDLabel, *fBGLabel;
	BRadioButton* fAudioCDRadio, *fDataCDRadio, *fMixCDRadio;
};


#endif	// _BURNVIEW_H_
