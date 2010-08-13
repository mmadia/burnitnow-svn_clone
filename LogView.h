/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _LOGVIEW_H_
#define _LOGVIEW_H_


#include <TextView.h>


class LogView : public BView {
public:
	LogView(BRect size);
	~LogView();
	BTextView* fLogTextView;
};


#endif	// _LOGVIEW_H_
