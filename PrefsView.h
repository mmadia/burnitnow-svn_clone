/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _PREFSVIEW_H_
#define _PREFSVIEW_H_


#include <Box.h>
#include <CheckBox.h>
#include <MenuField.h>


// Forward declaration of IconLabel class
class IconLabel;


class PrefsView : public BView {
public:
	PrefsView(BRect size);
	~PrefsView();

	BMenu* Recorders;
	BMenuField* RecordPop;
	BCheckBox* DAOOption, *BurnProof;
	BBox* MiscOpt;
	IconLabel* MiscLabel;
};


#endif	// _PREFSVIEW_H_
