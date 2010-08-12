/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "IconLabel.h"

#include <Application.h>
#include <Resources.h>


IconLabel::IconLabel(BRect size, char* labelstring, char* gfx)
	:
	BView(size, "IconLabel", B_FOLLOW_NONE, B_WILL_DRAW)
{
	strcpy(fLabel, labelstring);
	fBitmap = GetBitmapResource('BBMP', gfx);

}


IconLabel::~IconLabel()
{
	delete fBitmap;
}


void IconLabel::Draw(BRect frame)
{
	SetLowColor(216, 216, 216);
	DrawBitmap(fBitmap, BPoint(1, 0));
	SetFont(be_bold_font);
	DrawString(fLabel, BPoint(19, 12));
}


BBitmap* IconLabel::GetBitmapResource(type_code type, const char* name)
{
	size_t len = 0;
	BResources* rsrc = BApplication::AppResources();
	const void* data = rsrc->LoadResource(type, name, &len);

	if (data == NULL) {
		strcpy(fLabel, "FAN1");
		return NULL;
	}

	BMemoryIO stream(data, len);

	// Try to read as an archived bitmap.
	stream.Seek(0, SEEK_SET);
	BMessage archive;
	status_t err = archive.Unflatten(&stream);
	if (err != B_OK) {
		strcpy(fLabel, "FAN2");
		return NULL;
	}
	BBitmap* out = new BBitmap(&archive);
	if (!out) {
		strcpy(fLabel, "FAN3");
		return NULL;
	}
	err = (out)->InitCheck();
	if (err != B_OK) {
		delete out;
		out = NULL;
		strcpy(fLabel, "FAN4");
	}

	return out;
}
