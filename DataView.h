/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _DATAVIEW_H_
#define _DATAVIEW_H_


#include <Box.h>
#include <CheckBox.h>
#include <FilePanel.h>
#include <RadioButton.h>


// Forward declaration of IconLabel and jpButton classes
class IconLabel;
class jpButton;


class DataView : public BView {
public:
	DataView(BRect size);
	~DataView();

	jpButton* ChooseDir;
	jpButton* ChooseBootImage;
	char ChooseLabel[1024];
	BCheckBox* BootableCD;
	BFilePanel* FilePanel;

	IconLabel* FSLabel, *BootLabel;
	BBox* FSBox, *BootBox;
	BRadioButton* ISO9660, *Joliet, *Windows, *Mac, *BeOS, *Rock, *RealRock, *Own;
	jpButton* ChangeVolName;
};


#endif	// _DATAVIEW_H_
