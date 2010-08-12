/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _AUDIOVIEW_H_
#define _AUDIOVIEW_H_


#include <Box.h>
#include <CheckBox.h>
#include <TextView.h>


// Forward declaration of IconLabel class
class IconLabel;


class AudioView : public BView {
public:
	AudioView(BRect size);
	~AudioView();

	BCheckBox* Swab, *Nofix, *Preemp, *Pad;
	BBox* AdvOptions, *Info;
	IconLabel* AdvLabel;
	BTextView* InfoText;
	BScrollView* InfoScroll;
};


#endif	// _AUDIOVIEW_H_
