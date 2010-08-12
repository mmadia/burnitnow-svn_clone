/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _BOOTVIEW_H_
#define _BOOTVIEW_H_


#include <Box.h>
#include <CheckBox.h>
#include <MenuField.h>


// Forward declaration of IconLabel class
class IconLabel;


class BootView : public BView {
public:
	BootView(BRect size);
	~BootView();

	BMenu* Recorders;
	BMenuField* RecordPop;
	BCheckBox* DAOOption;
	BBox* MiscOpt;
	IconLabel* MiscLabel;
};


#endif	// _BOOTVIEW_H_
