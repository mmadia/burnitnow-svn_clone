/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _CDRWVIEW_H_
#define _CDRWVIEW_H_


#include <Box.h>
#include <Button.h>
#include <MenuField.h>
#include <Slider.h>


// Forward declaration of IconLabel
class IconLabel;


class CDRWView : public BView {
public:
	CDRWView(BRect size);
	~CDRWView();

	BSlider* fBlankSpeedSlider;
	BBox* fCDRWBox;
	IconLabel* fCDRWLabel;
	BMenu* fBlankMenu;
	BMenuField* fBlankMenuField;
	BButton* fBlankButton;
};


#endif	// _CDRWVIEW_H_
