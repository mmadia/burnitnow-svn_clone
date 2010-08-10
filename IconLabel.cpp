#include "IconLabel.h"

IconLabel::IconLabel (BRect size,char* labelstring,char* gfx) : BView(size,"IconLabel",B_FOLLOW_NONE,B_WILL_DRAW) {
	strcpy(label,labelstring);
	bitmap = GetBitmapResource('BBMP',gfx);

}
IconLabel::~IconLabel() {
	delete bitmap;
}
void IconLabel::Draw(BRect frame) {
	SetLowColor(216,216,216);
	DrawBitmap(bitmap,BPoint(1,0));
	SetFont(be_bold_font);
	DrawString(label,BPoint(19,12));
}
BBitmap* IconLabel::GetBitmapResource(type_code type,const char* name) {
	size_t len = 0;
	BResources *rsrc = BApplication::AppResources();
	const void *data = rsrc->LoadResource(type, name, &len);

	if (data == NULL) {
		strcpy(label,"FAN1");
		return NULL;
	}
	
	BMemoryIO stream(data, len);
	
	// Try to read as an archived bitmap.
	stream.Seek(0, SEEK_SET);
	BMessage archive;
	status_t err = archive.Unflatten(&stream);
	if (err != B_OK) {
		strcpy(label,"FAN2");
		return NULL;
	}
	BBitmap* out = new BBitmap(&archive);
	if (!out) {
		strcpy(label,"FAN3");
		return NULL;
	}
	err = (out)->InitCheck();
	if (err != B_OK) {
		delete out;
		out = NULL;
		strcpy(label,"FAN4");
	}
	
	return out;
}
