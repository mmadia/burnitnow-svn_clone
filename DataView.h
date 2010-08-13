/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _DATAVIEW_H_
#define _DATAVIEW_H_


#include <Box.h>
#include <Button.h>
#include <CheckBox.h>
#include <FilePanel.h>
#include <RadioButton.h>


// Forward declaration of IconLabel
class IconLabel;


class DataView : public BView {
public:
	DataView(BRect size);
	~DataView();

	BButton* fChooseDirButton;
	BButton* fChooseBootImageButton;
	char fChooseLabel[1024];
	BCheckBox* fBootableCDCheckBox;
	BFilePanel* fFilePanel;

	IconLabel* fFileSystemLabel, *fBootLabel;
	BBox* fFileSystemBox, *fBootBox;
	BRadioButton* fISO9660Radio, *fJolietRadio, *fWindowsRadio, *fMacRadio, *fBeOSRadio, *fRockRadio, *fRealRockRadio, *fOwnRadio;
	BButton* fChangeVolNameButton;
};


#endif	// _DATAVIEW_H_
