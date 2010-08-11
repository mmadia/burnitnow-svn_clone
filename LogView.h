/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _LOGVIEW_H_
#define _LOGVIEW_H_


#include <Be.h>
#include "const.h"
#include "IconLabel.h"


class LogView : public BView {
public:
	LogView(BRect size);
	~LogView();
	IconLabel* LogLabel;
	BBox* BoxLog;
	BTextView* LogText;
	BScrollView* LogScroll;
};


#endif	// _LOGVIEW_H_
