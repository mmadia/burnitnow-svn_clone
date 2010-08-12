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

	BMenu* fRecordersMenu;
	BMenuField* fRecordMenuField;
	BCheckBox* fDAOCheckBox, *fBurnProofCheckBox;
	BBox* fMiscOptBox;
	IconLabel* fMiscLabel;
};


#endif	// _PREFSVIEW_H_
