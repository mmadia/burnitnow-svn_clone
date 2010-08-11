/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include "RightList.h"
#include "jpWindow.h"


bool GlobalPattern = true;

extern bool VRCD;
extern char* IMAGE_NAME;
extern char* BURNIT_PATH;
extern char* BURN_DIR;


BBitmap* GetBitmapResource(type_code type, const char* name)
{
	size_t len = 0;
	BResources* rsrc = BApplication::AppResources();
	const void* data = rsrc->LoadResource(type, name, &len);

	if (data == NULL) {
		return NULL;
	}

	BMemoryIO stream(data, len);

	// Try to read as an archived bitmap.
	stream.Seek(0, SEEK_SET);
	BMessage archive;
	status_t err = archive.Unflatten(&stream);
	if (err != B_OK) {
		return NULL;
	}
	BBitmap* out = new BBitmap(&archive);
	if (!out) {
		return NULL;
	}
	err = (out)->InitCheck();
	if (err != B_OK) {
		delete out;
		out = NULL;
	}

	return out;
}


FileListItem::FileListItem(entry_ref* ref, char* name, BBitmap* icon)
	:
	BListItem()
{
	if (ref != NULL)
		fref = *ref;
	fname = new char[strlen(name)+1];
	strcpy(fname, name);
	ficon = icon;
	Pattern = GlobalPattern;
	GlobalPattern = !GlobalPattern;
}


FileListItem::~FileListItem()
{
	delete fname;
}


void FileListItem::DrawItem(BView* owner, BRect frame, bool complete)
{
	rgb_color rgbColor = {255, 255, 255};
	rgb_color rgbSelectedColor = {235, 235, 200};
	rgb_color rgbPatternColor = {244, 244, 255};
	rgb_color black = {0, 0, 0};



	if (IsSelected()) {
		rgbColor = rgbSelectedColor;
	} else {
		if (Pattern)
			rgbColor = rgbPatternColor;
	};

	owner->SetHighColor(rgbColor);
	owner->SetLowColor(rgbColor);
	owner->FillRect(frame);

	owner->SetHighColor(black);

	if (ficon != NULL)
		owner->DrawBitmap(ficon, BPoint(1, frame.top + 1));
	owner->MovePenTo(BPoint(21, frame.bottom - 1));
	owner->DrawString(fname);
}


RightList::RightList(BRect size)
	:
	BListView(size, "RightList", B_MULTIPLE_SELECTION_LIST, B_FOLLOW_NONE, B_WILL_DRAW)
{
	SetViewColor(255, 255, 255, 0);
	BDir = new BDirectory(BURN_DIR);
	TDir = new BDirectory(BURN_DIR);

	FileIcon = GetBitmapResource('BBMP', "BMP:FILEICON");
	DirIcon = GetBitmapResource('BBMP', "BMP:DIRICON");
	InfoIcon = GetBitmapResource('BBMP', "BMP:INFOICON");
	UpdateDir();

}


RightList::~RightList()
{
	delete BDir;
	delete TDir;
	delete FileIcon;
	delete DirIcon;
	delete InfoIcon;
}


void RightList::MessageReceived(BMessage* msg)
{
	entry_ref ref;
	int32 counter = 0;

	switch (msg->what) {

		case B_SIMPLE_DATA:
			if (VRCD) {
				while (msg->FindRef("refs", counter++, &ref) == B_OK) {
					MakeLink(&ref);
				}
				UpdateDir();
			} else {
				BAlert* MyAlert = new BAlert("BurnItNow", "You have to add a Virtual CD Directory to be able to drop files!", "Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
				MyAlert->Go();
			}
			break;

		default:
			BListView::MessageReceived(msg);
			break;
	}
}


void RightList::DeleteDirFromVRCD(entry_ref* ref)
{
	BDirectory* temp_dir;
	entry_ref temp_ref;
	temp_dir = new BDirectory(ref);
	while (temp_dir->GetNextRef(&temp_ref) != B_ENTRY_NOT_FOUND) {
		if (BEntry(&temp_ref, true).IsDirectory()) {
			DeleteDirFromVRCD(&temp_ref);
			BEntry(&temp_ref).Remove();
		} else {
			BEntry(&temp_ref).Remove();
		}

	}
	delete temp_dir;
}


void RightList::DeleteFromVRCD(entry_ref* ref)
{
	BEntry temp_entry(ref);

	if (temp_entry.IsDirectory()) {
		DeleteDirFromVRCD(ref);
		temp_entry.Remove();
	} else {
		temp_entry.Remove();
	}
}


void RightList::KeyDown(const char* bytes, int32 numBytes)
{
	BPath temp_path;
	int32 result;
	switch (bytes[0]) {
		case B_DELETE: {
				int32 selection = CurrentSelection();
				if (selection >= 0) {
					FileListItem* item = (FileListItem*)ItemAt(selection);
					BAlert* MyAlert = new BAlert("BurnItNow", "Are you sure you want to delete this selection", "Yes", "No", NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
					MyAlert->SetFeel(B_MODAL_ALL_WINDOW_FEEL);
					result = MyAlert->Go();
					if (result == 0) {
						DeleteFromVRCD(&item->fref);
						UpdateDir();

					} else {
						UpdateDir();
						WriteLog("Didnt delete file");
					}
					if (item != NULL) {
						delete item;
					}

				}

				break;
			}
		default: {
				BListView::KeyDown(bytes, numBytes);
			}
	}
}


void RightList::MouseDown(BPoint point)
{
	BMessage* msg = Window()->CurrentMessage();
	uint32 clicks = msg->FindInt32("clicks");
	uint32 button = msg->FindInt32("buttons");
	if ((button == mLastButton) && (clicks > 1)) {
		mClickCount++;
	} else {
		mClickCount = 1;
	}
	mLastButton = button;
	if ((button == B_PRIMARY_MOUSE_BUTTON) && (mClickCount == 2)) {
		int32 selection = CurrentSelection();
		if (selection >= 0) {
			FileListItem* item = (FileListItem*)ItemAt(selection);
			if (item->ficon != NULL && item->ficon != InfoIcon)
				if (BDir->SetTo(&item->fref) == B_OK) {
					UpdateDir();
				}
		}
		mClickCount = 0;

	} else {
		BListView::MouseDown(point);
	}
}


void RightList::UpdateInfo(char* str1 = NULL, char* str2 = NULL, char* str3 = NULL, char* str4 = NULL, char* str5 = NULL)
{
	MakeEmpty();
	if (str1 != NULL) {
		AddItem(new FileListItem(NULL, str1, InfoIcon));
	}
	if (str2 != NULL) {
		AddItem(new FileListItem(NULL, str2, NULL));
	}
	if (str3 != NULL) {
		AddItem(new FileListItem(NULL, str3, NULL));
	}
	if (str4 != NULL) {
		AddItem(new FileListItem(NULL, str4, NULL));
	}
	if (str5 != NULL) {
		AddItem(new FileListItem(NULL, str5, NULL));
	}

}


void RightList::UpdateDir()
{
	BEntry temp_entry;
	entry_ref temp_ref;
	MakeEmpty();
	BDir->GetEntry(&temp_entry);
	BDir->SetTo(&temp_entry);
	while (BDir->GetNextRef(&temp_ref) != B_ENTRY_NOT_FOUND) {
		if (BEntry(&temp_ref, true).IsDirectory()) {
			AddItem(new FileListItem(&temp_ref, temp_ref.name, DirIcon));
		} else {
			AddItem(new FileListItem(&temp_ref, temp_ref.name, FileIcon));
		}

	}

}


void RightList::RemoveVRCD()
{
}


void RightList::ParentDir()
{
	BEntry temp_entry, temp_entry2;
	BPath temp_path;
	if (BDir->GetEntry(&temp_entry) == B_OK) {
		temp_entry.GetParent(&temp_entry2);
		if (temp_entry2.GetPath(&temp_path) == B_OK)
			if (!strncmp(temp_path.Path(), BURN_DIR, strlen(BURN_DIR))) {
				temp_entry.GetParent(BDir);
				UpdateDir();
			} else {
			}
	}
}


void RightList::CreateDir()
{
	MakeDirWindow = new AskName(BRect(200, 200, 440, 240), "Make directory", MAKE_DIRECTORY, "");
	MakeDirWindow->Show();
}


void RightList::MakeDir(const char* name)
{
	char temp_char[2048];
	BEntry temp_entry;
	BPath temp_path;
	BDir->GetEntry(&temp_entry);
	temp_entry.GetPath(&temp_path);
	sprintf(temp_char, "%s/%s", temp_path.Path(), name);
	if (!BEntry(temp_char).Exists()) {
		BDir->CreateDirectory(temp_char, NULL);
		UpdateDir();
	} else {
		BAlert* MyAlert = new BAlert("BurnItNow", "The directory is already exists!", "Ok", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
		MyAlert->SetFeel(B_MODAL_ALL_WINDOW_FEEL);
		MyAlert->Go();
	}

}


void RightList::MakeDir(entry_ref* ref)
{
	entry_ref temp_ref;
	char temp_char[4096];
	BPath temp_path;
	BDirectory* temp_dir;
	BEntry temp_entry;
	temp_dir = new BDirectory(ref);
	while (temp_dir->GetNextRef(&temp_ref) == B_OK) {
		if (BEntry(&temp_ref, false).IsDirectory()) {
			TDir->GetEntry(&temp_entry);
			temp_entry.GetPath(&temp_path);
			memset(temp_char, 0, sizeof(temp_char));
			sprintf(temp_char, "%s/%s", temp_path.Path(), temp_ref.name);
			if (TDir->CreateDirectory(temp_char, NULL) == B_OK)
				if (TDir->SetTo(temp_char) == B_OK) {
					MakeDir(&temp_ref);
					TDir->SetTo(temp_path.Path());
				}


		} else {
			if (!BEntry(&temp_ref, true).IsDirectory()) {
				if (TDir->GetEntry(&temp_entry) == B_OK)
					if (temp_entry.GetPath(&temp_path) == B_OK) {
						sprintf(temp_char, "%s/%s", temp_path.Path(), temp_ref.name);
						if (BEntry(&temp_ref, false).GetPath(&temp_path) == B_OK) {
							if (TDir->CreateSymLink(temp_char, temp_path.Path(), NULL) == B_OK) {
							}
						}
					}

			} else {
				// Don't follow linked directorys
				WriteLog("RightList::MakeDir -> #1");
			}
		}
	}
	delete temp_dir;
}


void RightList::MakeLink(entry_ref* ref)
{
	entry_ref temp_ref;
	char temp_char[4096];
	BPath temp_path;
	BEntry temp_entry;

	if (BEntry(ref, false).IsDirectory()) {
		BDir->GetEntry(&temp_entry);
		temp_entry.GetPath(&temp_path);
		sprintf(temp_char, "%s/%s", temp_path.Path(), ref->name);
		if (TDir->CreateDirectory(temp_char, NULL) == B_OK)
			if (TDir->SetTo(temp_char) == B_OK) {
				MakeDir(ref);
				TDir->Rewind();
			}
	} else {
		if (!BEntry(ref, true).IsDirectory()) {
			if (BDir->GetEntry(&temp_entry) == B_OK) {
				if (temp_entry.GetPath(&temp_path) == B_OK) {
					sprintf(temp_char, "%s/%s", temp_path.Path(), ref->name);
					if (BEntry(ref, false).GetPath(&temp_path) == B_OK) {
						if (BDir->CreateSymLink(temp_char, temp_path.Path(), NULL) == B_OK) {
						}
					}
				}
			}
		} else {
			// Don't follow linked directorys
			WriteLog("RightList::MakeLink -> #1");
		}
	}

}


void RightList::WriteLog(char* string)
{
	jpWindow* win = dynamic_cast<jpWindow*>(Window());
	if (win != NULL) {
		win->MessageLog(string);
	}
}
