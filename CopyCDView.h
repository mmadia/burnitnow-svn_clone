/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _COPYCDVIEW_H_
#define _COPYCDVIEW_H_


#include <Be.h>
#include "const.h"
#include "jpButton.h"


class CopyCDView : public BView {
public:
	CopyCDView(BRect size, BWindow* twin);
	~CopyCDView();
	void MessageReceived(BMessage* message);

private:
	BWindow* tempwin;
	jpButton* OpenDir;
};


#endif	// _COPYCDVIEW_H_
