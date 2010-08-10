/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include <Be.h>
#include "const.h"
#ifndef __LEFT_LIST__
#define __LEFT_LIST__

struct AudioInfo {
	int32 bps;
	int32 frame_rate;
	int32 channels;
	char pretty_name[200];
	char short_name[100];
	bigtime_t total_time;
};

class LeftListItem : public BListItem {
		public:
		LeftListItem(entry_ref *ref, char *name, BBitmap *icon, struct AudioInfo *Info);
		~LeftListItem();
		virtual void DrawItem(BView *owner, BRect frame, bool complete);
		
		BBitmap *ficon;
		char fname[1024];
		entry_ref fref;
		struct AudioInfo AInfo;
		bool isAudio;
		

};

class LeftList : public BListView {
	public:
		LeftList(BRect size);
		~LeftList();
		virtual void KeyDown(const char* bytes, int32 numBytes);
		virtual void MouseDown(BPoint point);
		virtual void WriteLog(char *string); 
		virtual void MessageReceived(BMessage *message);
		
		uint32 mLastButton,mClickCount;
		BBitmap *ISOIcon;
		BBitmap *VRCDIcon;
		BBitmap *AudioIcon;
		BPopUpMenu *TrackPop;

};
#endif
