/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _CDRWVIEW_H_
#define _CDRWVIEW_H_


#include <Be.h>
#include "const.h"
#include "IconLabel.h"
#include "jpButton.h"


class CDRWView : public BView {
public:
	CDRWView(BRect size);
	~CDRWView();

	BSlider* BlankSpeed;
	BBox* CDRWBox;
	IconLabel* CDRWLabel;
	BMenu* Blank;
	BMenuField* BlankPop;
	jpButton* BlankButton;
};


#endif	// _CDRWVIEW_H_
