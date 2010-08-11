/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _ICONLABEL_H_
#define _ICONLABEL_H_


#include <Be.h>


class IconLabel : public BView {
public:
	IconLabel(BRect size, char* labelstring, char* gfx);
	~IconLabel();
	virtual void Draw(BRect frame);
	BBitmap* GetBitmapResource(type_code type, const char* name);

private:
	const BBitmap* bitmap;
	char label[100];
};


#endif	// _ICONLABEL_H_
