/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _BOOTVIEW_H_
#define _BOOTVIEW_H_


#include <Be.h>
#include "const.h"
#include "IconLabel.h"


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
