/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef __ICONLABEL__
#define __ICONLABEL__

#include <Be.h>

class IconLabel : public BView {
	public:
		IconLabel(BRect size,char* labelstring,char* gfx);
		~IconLabel();
		virtual void Draw(BRect frame);
		BBitmap* GetBitmapResource(type_code type,const char* name);
	private:
		const BBitmap* bitmap;
		char label[100];
};
#endif
