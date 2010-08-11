/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "LeftList.h"
#include "jpWindow.h"


extern bool VRCD;
extern bool ISOFILE;
extern bool SHOW_INFO;

extern char* IMAGE_NAME;
extern char* BURNIT_PATH;
extern char* BURN_DIR;


BBitmap* GetBitmapResource(type_code type, const char* name);

LeftListItem::LeftListItem(entry_ref* ref, char* name, BBitmap* icon, struct AudioInfo* Info)
	:
	BListItem()
{
	fref = *ref;
	strcpy(fname, name);
	ficon = icon;

	if (Info != NULL) {
		isAudio = true;
		AInfo.bps = Info->bps;
		AInfo.frame_rate = Info->frame_rate;
		AInfo.channels = Info->channels;
		AInfo.total_time = Info->total_time;
		strcpy(AInfo.pretty_name, Info->pretty_name);
		strcpy(AInfo.short_name, Info->short_name);
	} else {
		isAudio = false;
	}


}


LeftListItem::~LeftListItem()
{
}


void LeftListItem::DrawItem(BView* owner, BRect frame, bool complete)
{
	char temp_char[1024];
	rgb_color rgbColor = {255, 255, 255};
	rgb_color rgbSelectedColor = {235, 235, 200};
	rgb_color rgbPatternColor = {244, 244, 255};
	rgb_color black = {0, 0, 0};



	if (IsSelected()) {
		rgbColor = rgbSelectedColor;
	} else {
		if (!isAudio)
			rgbColor = rgbPatternColor;
	};

	owner->SetHighColor(rgbColor);
	owner->SetLowColor(rgbColor);
	owner->FillRect(frame);

	owner->SetHighColor(black);


	owner->DrawBitmap(ficon, BPoint(1, frame.top + 1));
	owner->MovePenTo(BPoint(21, frame.bottom - 1));
	if (isAudio) {
		sprintf(temp_char, "%s - %d:%d", fname, (int)(AInfo.total_time / 1000000) / 60, (int)(AInfo.total_time / 1000000) % 60);
		owner->DrawString(temp_char);
	} else {
		owner->DrawString(fname);
	}
}


LeftList::LeftList(BRect size)
	:
	BListView(size, "LeftList", B_MULTIPLE_SELECTION_LIST, B_FOLLOW_NONE, B_WILL_DRAW)
{
	SetViewColor(255, 255, 255, 0);

	VRCDIcon = GetBitmapResource('BBMP', "BMP:VRCDICON");
	ISOIcon = GetBitmapResource('BBMP', "BMP:ISOICON");
	AudioIcon = GetBitmapResource('BBMP', "BMP:AUDIOICON");

	TrackPop = new BPopUpMenu("Tracks Popup");
	TrackPop->SetRadioMode(false);
	TrackPop->AddItem(new BMenuItem("Move Up", new BMessage('mvup')));
	TrackPop->AddItem(new BMenuItem("Remove", new BMessage('remt')));
	TrackPop->AddItem(new BMenuItem("Move Down", new BMessage('mvdn')));
	TrackPop->AddItem(new BMenuItem("Play", new BMessage('play')));

}


LeftList::~LeftList()
{
	delete VRCDIcon;
	delete ISOIcon;
	delete AudioIcon;
}


void LeftList::MessageReceived(BMessage* msg)
{
	struct AudioInfo AInfo;
	BMediaFile* testfile;
	bool isAudio = false;
	BMediaTrack* track;
	media_codec_info codecInfo;
	media_format format;
	memset(&format, 0, sizeof(format));

	entry_ref ref;
	int32 counter = 0;

	switch (msg->what) {

		case B_SIMPLE_DATA:
			while (msg->FindRef("refs", counter++, &ref) == B_OK) {
				if ((testfile = new BMediaFile(&ref)) != NULL) {
					testfile->InitCheck();
					track = testfile->TrackAt(0);
					if (track != NULL) {
						track->EncodedFormat(&format);
						if (format.IsAudio()) {
							memset(&format, 0, sizeof(format));
							format.type = B_MEDIA_RAW_AUDIO;
							track->DecodedFormat(&format);
							AInfo.total_time = track->Duration();
							media_raw_audio_format* raf = &(format.u.raw_audio);
							AInfo.bps = (int32)(raf->format & 0xf);
							AInfo.frame_rate = (int32)raf->frame_rate;
							AInfo.channels = (int32)raf->channel_count;
							track->GetCodecInfo(&codecInfo);
							strcpy(AInfo.pretty_name, codecInfo.pretty_name);
							strcpy(AInfo.short_name, codecInfo.short_name);
							isAudio = true;
						}
					}
				} else {
					WriteLog("MediaFile NULL (file doesnt exists!?)");
				}
				delete testfile;
				if (isAudio) {
					if (!strcmp(AInfo.pretty_name, "Raw Audio") && (AInfo.channels == 2) && (AInfo.frame_rate == 44100) && (AInfo.bps == 2)) {
						AddItem(new LeftListItem(&ref, ref.name, AudioIcon, &AInfo));
					} else {
						BAlert* MyAlert = new BAlert("BurnItNow", "You can only burn 16 bits stereo 44.1 kHz Raw Audio files.\n (More audio files will be supported in the future)", "Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
						MyAlert->Go();
					}
				} else {
					BPath temp_path;
					BEntry(&ref).GetPath(&temp_path);
					jpWindow* win = dynamic_cast<jpWindow*>(Window());
					if (win != NULL) {
						win->SetISOFile((char*)temp_path.Path());
					}
				}
			}
			break;
		default:
			BListView::MessageReceived(msg);
			break;
	}
}


void LeftList::KeyDown(const char* bytes, int32 numBytes)
{
	BPath temp_path;
	int32 result;
	switch (bytes[0]) {
		case B_DELETE: {
				int32 selection = CurrentSelection();
				if (selection >= 0) {
					BAlert* MyAlert = new BAlert("BurnItNow", "Are you sure you want to delete this selection", "Yes", "No", NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
					MyAlert->SetFeel(B_MODAL_ALL_WINDOW_FEEL);
					result = MyAlert->Go();
					if (result == 0) {
						LeftListItem* item = (LeftListItem*)RemoveItem(selection);
						if (!item->isAudio) {
							VRCD = false;
							ISOFILE = false;
							jpWindow* win = dynamic_cast<jpWindow*>(Window());
							if (win != NULL) {
								win->MakeDir->SetEnabled(false);
								win->ParentDir->SetEnabled(false);
								win->NewVRCD->SetEnabled(true);
								win->AddISO->SetEnabled(true);
								win->DataV->BootableCD->SetEnabled(false);
								win->DataV->ChooseBootImage->SetEnabled(false);
							}

						}
						if (item != NULL) {
							delete item;
						}
					} else {}

				}

				break;
			}
		default: {
				BListView::KeyDown(bytes, numBytes);
			}
	}

}


void LeftList::MouseDown(BPoint point)
{
	char temp1[150];
	char temp2[150];
	char temp3[150];
	char temp4[150];
	char temp5[150];
	BMessage* msg = Window()->CurrentMessage();
	uint32 clicks = msg->FindInt32("clicks");
	uint32 button = msg->FindInt32("buttons");


	if ((button == mLastButton) && (clicks > 1)) {
		mClickCount++;
	} else {
		mClickCount = 1;
	}
	mLastButton = button;

	if ((button == B_SECONDARY_MOUSE_BUTTON)) {
		int32 itemn = IndexOf(point);
		if (itemn >= 0) {
			BMenuItem* selected;
			BPoint p = point;
			ConvertToScreen(&p);
			Select(itemn);
			selected = TrackPop->Go(p);
			if (selected) {
				int32 selection = CurrentSelection();
				if (!strcmp(selected->Label(), "Move Up")) {
					LeftListItem* item = (LeftListItem*)ItemAt(selection - 1);
					LeftListItem* item2 = (LeftListItem*)ItemAt(selection);
					if ((selection - 1 > 0) && (item->isAudio) && (item2->isAudio))
						SwapItems(selection, selection - 1);


				} else if (!strcmp(selected->Label(), "Move Down")) {
					LeftListItem* item = (LeftListItem*)ItemAt(selection);
					if ((selection + 1 <= CountItems()) && (item->ficon == AudioIcon))
						SwapItems(selection, selection + 1);

				} else if (!strcmp(selected->Label(), "Play")) {
					LeftListItem* item = (LeftListItem*)ItemAt(selection);
					if (item->ficon == AudioIcon)
						be_roster->Launch(&item->fref);
				} else if (!strcmp(selected->Label(), "Remove")) {
					int32 result;
					BAlert* MyAlert = new BAlert("BurnItNow", "Are you sure you want to delete this selection", "Yes", "No", NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
					MyAlert->SetFeel(B_MODAL_ALL_WINDOW_FEEL);
					result = MyAlert->Go();
					if (result == 0) {
						LeftListItem* item = (LeftListItem*)RemoveItem(itemn);
						if (!item->isAudio) {
							VRCD = false;
							ISOFILE = false;
							jpWindow* win = dynamic_cast<jpWindow*>(Window());
							if (win != NULL) {
								win->MakeDir->SetEnabled(false);
								win->ParentDir->SetEnabled(false);
								win->NewVRCD->SetEnabled(true);
								win->AddISO->SetEnabled(true);

							}

						}
						if (item != NULL) {
							delete item;
						}
					} else {}
				}

			}
			return;
		}
	}
	if ((button == B_PRIMARY_MOUSE_BUTTON) && (mClickCount == 2)) {
		int32 selection = CurrentSelection();
		if (selection >= 0) {
			LeftListItem* item = (LeftListItem*)ItemAt(selection);
			if (item->isAudio) {
				sprintf(temp1, "%s", item->AInfo.pretty_name);
				sprintf(temp2, "%d Channels", (int)item->AInfo.channels);
				sprintf(temp3, "%.1f kHz", ((float)item->AInfo.frame_rate / (float)1000));
				sprintf(temp4, "%d bits", (int)item->AInfo.bps * 8);
				sprintf(temp5, "Info on AudioFile");
				jpWindow* win = dynamic_cast<jpWindow*>(Window());
				if (win != NULL) {
					win->RList->UpdateInfo(temp5, temp1, temp2, temp3, temp4);
				}
			}
			if (item->ficon == VRCDIcon) {
				jpWindow* win = dynamic_cast<jpWindow*>(Window());
				if (win != NULL) {
					win->RList->UpdateDir();
				}
			}
			if (item->ficon == ISOIcon) {
				sprintf(temp1, "Info on ISOFile");
				sprintf(temp2, "%s", item->fname);
				sprintf(temp3, "X Mb");
				jpWindow* win = dynamic_cast<jpWindow*>(Window());
				if (win != NULL) {
					win->RList->UpdateInfo(temp1, temp2, temp3, NULL, NULL);
				}
			}
		}
		mClickCount = 0;

	} else {
		BListView::MouseDown(point);
	}
}


void LeftList::WriteLog(char* string)
{
	jpWindow* win = dynamic_cast<jpWindow*>(Window());
	if (win != NULL) {
		win->PutLog(string);
	}
}
