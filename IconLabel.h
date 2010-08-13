/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _ICONLABEL_H_
#define _ICONLABEL_H_


#include <Bitmap.h>
#include <View.h>


class IconLabel : public BView {
public:
	IconLabel(BRect size, const char* labelstring, const char* gfx);
	~IconLabel();
	virtual void Draw(BRect frame);
	BBitmap* GetBitmapResource(type_code type, const char* name);

private:
	const BBitmap* fBitmap;
	char fLabel[100];
};


#endif	// _ICONLABEL_H_
