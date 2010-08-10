#include <Be.h>
#include "AskName.h"
#include "const.h"
#ifndef __RIGHT_LIST__
#define __RIGHT_LIST__

class FileListItem : public BListItem {
		public:
		FileListItem(entry_ref *ref, char *name, BBitmap *icon);
		~FileListItem();
		virtual void DrawItem(BView *owner, BRect frame, bool complete);
		
		BBitmap *ficon;
		char *fname;
		entry_ref fref;
		bool Pattern;

};

class RightList : public BListView {
	public:
		RightList(BRect size);
		~RightList();
		virtual void UpdateInfo(char *str1, char *str2, char *str3, char *str4, char *str5);
		virtual void UpdateDir();
		virtual void KeyDown(const char* bytes, int32 numBytes);
		virtual void MouseDown(BPoint point);
		virtual void RemoveVRCD();
		virtual void ParentDir();
		virtual void CreateDir();
		virtual void MakeLink(entry_ref *ref);
		virtual void MakeDir(entry_ref *ref);
		virtual void MakeDir(const char *name);
		virtual void WriteLog(char *string); 
		virtual void MessageReceived(BMessage *message);
		virtual void DeleteFromVRCD(entry_ref *ref);
		virtual void DeleteDirFromVRCD(entry_ref *ref);
		
		BDirectory *BDir, *TDir;
		uint32 mLastButton,mClickCount;
		AskName *MakeDirWindow;
		BBitmap *FileIcon;
		BBitmap *DirIcon;
		BBitmap *InfoIcon;

};
#endif