/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */


#include <TextView.h>
#include <String.h>
#include <stdio.h>
#include <stdlib.h>
#include "jpWindow.h"
#include "MakeBFS.h"


#define VERSION "beta 2"


const char* BlType[] = {"all", "fast", "session", "track", "trtail", "unreserve", "unclose"};
int16 BLANK_TYPE;
int16 BLANK_SPD;
int16 BURN_SPD;
int16 BURN_TYPE;
int16 IMAGE_TYPE;
int16 SCSI_DEV;
int16 CDSIZE;

uint32 nrtracks;
float angles[100];

extern BDirectory* BFSDir;
bool VRCD;
bool ISOFILE;
bool ONTHEFLY;
bool BOOTABLE;
entry_ref BOOTIMAGEREF;

char* IMAGE_NAME;
char* BURNIT_PATH;
char* BURN_DIR;


char BURNPROOF[30]; // driveropts = burnproof
char PAD[10]; // -pad (audio)
char DAO[10]; // -dao
char NOFIX[10]; // -nofix (audio)
char PREEMP[10]; // -preemp (audio)
char SWAB[10]; // -swab
char BOOTSTRING[128]; // -c boot.catalog -b boot.catalog/boot.img

char MULTISESSION[10];
char AUDIO_FILES[10240];
char DUMMYMODE[10];
char EJECT[10];
char ISOFILE_DIR[1024];
char DATA_STRING[100];
char VOL_NAME[25];
bool JUST_IMAGE;

bool scsibus = false;

FILE* CLInput;
thread_id OPMkImage = 0, Cntrl = 0, OPBurn = 0, OPBlank = 0;


int32 controller(void* p)
{
	char* command = (char*)p;
	CLInput = popen(command, "r");
	return 0;
}


int32 OutPutMkImage(void* p)
{
	char buffer[1024], buf[10], buf2[10], buf3[100];
	int temp, i;
	StatusWindow* SWin = (StatusWindow*)p;
	while (!feof(CLInput) && !ferror(CLInput)) {
		buffer[0] = 0;

		fgets(buffer, 1024, CLInput);
		if (buffer[6] == '%' || buffer[7] == '%' || buffer[8] == '%') {
			strncpy(&buf[0], &buffer[0], 8);

			for (i = 0; i < (int)strlen(buffer); i++) {
				if (buffer[i] == '.')
					break;
			}
			sprintf(buf3, "Making Image %s", buf);
			strncpy(&buf2[0], &buffer[1], i - 1);
			temp = atoi(buf2);
			SWin->Lock();
			SWin->UpdateStatus(temp, buf3);
			SWin->Unlock();
		}

	};

	if (!JUST_IMAGE) {
		SWin->Lock();
		SWin->SendMessage(new BMessage(BURN_WITH_CDRECORD));
		SWin->Unlock();
	} else {
		SWin->Lock();
		SWin->Ready();
		SWin->Unlock();
		sprintf(IMAGE_NAME, "%s/tmp/BurnIT.raw", BURNIT_PATH);
	}
	if (BOOTABLE) {
		char temp[1024];
		BPath path;
		BEntry(&BOOTIMAGEREF, true).GetPath(&path);
		sprintf(temp, "%s/boot.catalog/%s", BURN_DIR, path.Leaf());
		BEntry(temp, true).Remove();
		sprintf(temp, "%s/boot.catalog", BURN_DIR);
		BEntry(temp, true).Remove();

	}
	return 0;
}


int32 OutPutBurn(void* p)
{
	bool progress_mode = false;
	char buffer[1024], buf[50];
	int done;
	int left;
	bool noerror = true;
	StatusWindow* SWin = (StatusWindow*)p;
	while (!feof(CLInput) && !ferror(CLInput)) {

		buffer[0] = 0;
		if (!progress_mode) {
			fgets(buffer, 1024, CLInput);
		} else {
			fread(buffer, 1, 33, CLInput);
			buffer[33] = 0;
			if (buffer[0] != 0x0d) progress_mode = false;

		}
		if (!strncmp(buffer, "Starting new", 12)) {
			progress_mode = true;
		}
		if (!strncmp(buffer, "Sense Code:", 11)) {
			SWin->Lock();
			BMessage hejsan(WRITE_TO_LOG);
			SWin->StatusSetText("An error accord when burning!");
			hejsan.AddString("Text", "An error accord when burning, the most common wrong is that its not an empty or a damaged cd.\nTry again and if it fails send a email to jojje@quake.swe.net and write exactly what you did in the email.\n\n Thanks for using BurnIT NOW!");
			hejsan.AddBool("Error", true);
			SWin->SendMessage(&hejsan);
			noerror = false;
			SWin->Unlock();
			break;
		}
		if (!strncmp(buffer, "Last chance", 11)) {
			SWin->Lock();
			SWin->StatusSetText("Preparing...");
			SWin->Unlock();
		};
		if (!strncmp(&buffer[1], "Track ", 6)) {
			strncpy(&buf[0], &buffer[11], 3);
			done = strtol(buf, 0, 0);
			strncpy(&buf[0], &buffer[18], 3);
			left = strtol(buf, 0, 0);
			if (done) {
				sprintf(buf, "Burning %d of %d done", done, left);
				SWin->Lock();
				SWin->StatusSetMax((float)left);
				SWin->UpdateStatus((float)done, &buffer[1]);
				SWin->Unlock();
			}
		}
		if (!strncmp(buffer, "Fixating", 8)) {

			SWin->Lock();
			SWin->StatusSetColor(black);
			SWin->StatusUpdateReset();
			SWin->StatusSetText("Fixating...");
			SWin->Unlock();
		}

	};
	SWin->Lock();
	if (!noerror) {
		SWin->StatusSetColor(red);
		SWin->StatusUpdateReset();
		SWin->Ready();
		SWin->UpdateStatus(100, "An error accord when burning!");
	}
	SWin->SendMessage(new BMessage(SET_BUTTONS_TRUE));
	if (noerror)
		SWin->Ready();
	SWin->Unlock();
	sprintf(IMAGE_NAME, "%s/tmp/BurnIT.raw", BURNIT_PATH);
	BEntry(IMAGE_NAME, true).Remove();
	if (BOOTABLE) {
		char temp[1024];
		BPath path;
		BEntry(&BOOTIMAGEREF, true).GetPath(&path);
		sprintf(temp, "%s/boot.catalog/%s", BURN_DIR, path.Leaf());
		BEntry(temp, true).Remove();
		sprintf(temp, "%s/boot.catalog", BURN_DIR);
		BEntry(temp, true).Remove();
	}
	return 0;
}


int32 OutPutBlank(void* p)
{
	char buffer[1024];
	bool temp = true;
	StatusWindow* SWin = (StatusWindow*)p;

	while (!feof(CLInput) && !ferror(CLInput)) {
		buffer[0] = 0;
		fgets(buffer, 1024, CLInput);

		snooze(500000);
		SWin->Lock();
		if (temp) {
			SWin->StatusSetColor(green);
			temp = false;

		} else {
			SWin->StatusSetColor(blue);
			temp = true;
		}
		SWin->StatusUpdateReset();
		SWin->StatusSetText("Blanking...");


		SWin->Unlock();
	};
	SWin->Lock();
	SWin->Ready();
	SWin->StatusSetText("Blanking Done!");
	SWin->SendMessage(new BMessage(SET_BUTTONS_TRUE));
	SWin->Unlock();
	return 0;
}


jpWindow::jpWindow(BRect frame)
	:
	BWindow(frame, "BurnIT NOW!", B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_NOT_RESIZABLE)
{
	char temp_char[1024];
	entry_ref temp_ref;
	BRect r;
	BMenu* menu;
	SetTitle("BurnIt NOW!");
	BurnITPrefs = new Prefs("BurnIT NOW!");
	// Load settings
	InitBurnIT();

	r = Bounds();
	r.top = 20;
	AddChild(BoxAround = new BBox(r, "BoxAround", B_FOLLOW_NONE, B_WILL_DRAW | B_FRAME_EVENTS, B_PLAIN_BORDER));
	// Size Status and button
	r = BoxAround->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = r.bottom - 30;
	r.right -= 55;
	MyStatus = new BStatusBar(r, "MyStatus");
	MyStatus->SetMaxValue(100.0);
	BoxAround->AddChild(MyStatus);
	MyStatus->Reset();

	r = BoxAround->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = r.bottom - 17;
	r.left = r.right - 50;
	CalcSize = new jpButton(r, "calcsize", "Calc. Size", new BMessage(CALCULATE_SIZE));
	BoxAround->AddChild(CalcSize);

	// Tabs
	r = BoxAround->Bounds();
	r.InsetBy(5.0, 5.0);
	r.bottom = 210;

	MyTab = new BTabView(r, "mytabview", B_WIDTH_FROM_LABEL);
	MyTab->SetViewColor(216, 216, 216, 0);

	r = MyTab->Bounds();
	r.InsetBy(5.0, 5.0);
	r.bottom -= MyTab->TabHeight();
	MiscTab = new BTab();
	BurnV = new BurnView(r);
	MyTab->AddTab(BurnV, MiscTab);
	MiscTab->SetLabel("Burn");

	MiscTab = new BTab();
	DataV = new DataView(r);
	MyTab->AddTab(DataV, MiscTab);
	MiscTab->SetLabel("Data");

	MiscTab = new BTab();
	AudioV = new AudioView(r);
	MyTab->AddTab(AudioV, MiscTab);
	MiscTab->SetLabel("Audio");


	MiscTab = new BTab();
	MyTab->AddTab(new CopyCDView(r, this), MiscTab);
	MiscTab->SetLabel("CopyCD");


	MiscTab = new BTab();
	CDRWV = new CDRWView(r);
	MyTab->AddTab(CDRWV, MiscTab);
	MiscTab->SetLabel("CDRW");

	MiscTab = new BTab();
	PrefsV = new PrefsView(r);
	MyTab->AddTab(PrefsV, MiscTab);
	MiscTab->SetLabel("Prefs");

	MiscTab = new BTab();
	LogV = new LogView(r);
	MyTab->AddTab(LogV, MiscTab);
	MiscTab->SetLabel("Log");

	BoxAround->AddChild(MyTab);




	// FileList(RightList)
	r = BoxAround->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = 260;
	r.bottom = r.bottom - 50;
	r.left = (r.right / 2) + 3;
	r.right -= (B_V_SCROLL_BAR_WIDTH + 3);
	RList = new RightList(r);

	BoxAround->AddChild(new BScrollView("Scroll files/info", RList, B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, false, true));

	// ParentDir button
	r = BoxAround->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = 220;
	r.bottom = 240;
	r.left = (r.right / 2);
	r.right -= (r.right - r.left) / 2;
	MakeDir = new jpButton(r, "ParentDir", "ParentDir", new BMessage(PARENT_DIR));
	BoxAround->AddChild(MakeDir);

	// MakeDir button
	r = BoxAround->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = 220;
	r.bottom = 240;
	r.left = (r.right / 2) + 2;
	r.left += (r.right - r.left) / 2 + 2;
	r.right -= 2;
	ParentDir = new jpButton(r, "MakeDir", "MakeDir", new BMessage(MAKE_DIR));
	BoxAround->AddChild(ParentDir);

	if (!VRCD) {
		MakeDir->SetEnabled(false);
		ParentDir->SetEnabled(false);
	}

	// LeftList
	r = BoxAround->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = 260;
	r.bottom = r.bottom - 50;
	r.right -= ((r.right / 2) + (B_V_SCROLL_BAR_WIDTH) + 3);
	LList = new LeftList(r);

	BoxAround->AddChild(new BScrollView("Scroll LeftListr", LList, B_FOLLOW_LEFT | B_FOLLOW_TOP, 0, false, true));

	// Init
	if (VRCD) {
		sprintf(temp_char, "VRCD - [%s]", VOL_NAME);
		if (BOOTABLE)
			sprintf(temp_char, "VRCD(Boot) - [%s]", VOL_NAME);
		else
			sprintf(temp_char, "VRCD - [%s]", VOL_NAME);

		LList->AddItem(new LeftListItem(&temp_ref, temp_char, LList->VRCDIcon, NULL), 0);
	}
	if (ISOFILE && strcmp(ISOFILE_DIR, "NONE")) {
		BEntry(ISOFILE_DIR).GetRef(&temp_ref);
		LList->AddItem(new LeftListItem(&temp_ref, temp_ref.name, LList->ISOIcon, NULL), 0);
	}
	CDRWV->Blank->ItemAt(BLANK_TYPE)->SetMarked(true);

	// New VRCD
	r = BoxAround->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = 220;
	r.bottom = 240;
	r.right = 100;
	NewVRCD = new jpButton(r, "New Virtual CD", "New Virtual CD", new BMessage(NEW_VRCD));
	BoxAround->AddChild(NewVRCD);

	// Open ISO
	r = BoxAround->Bounds();
	r.InsetBy(5.0, 5.0);
	r.top = 220;
	r.bottom = 240;
	r.left = 105;
	r.right -= ((r.right / 2) + (B_V_SCROLL_BAR_WIDTH) + 2);
	AddISO = new jpButton(r, "Add ISOFile", "Add ISOFile", new BMessage(OPEN_ISO_FILE));
	BoxAround->AddChild(AddISO);

	// FilePanel open isofile
	OpenISOPanel = new BFilePanel(B_OPEN_PANEL, &be_app_messenger, NULL, B_FILE_NODE, false, NULL, NULL, true, true);
	SaveISOPanel = new BFilePanel(B_SAVE_PANEL, &be_app_messenger, NULL, B_FILE_NODE, false, NULL, NULL, true, true);

	// MenuBar
	r = Bounds();
	r.top = 0;
	r.bottom = 19;
	menubar = new BMenuBar(r, "menu_bar");
	AddChild(menubar);

	menu = new BMenu("File");
	menu->AddItem(new BMenuItem("About", new BMessage(MENU_FILE_ABOUT), 'A'));
	menu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED), 'Q'));
	menubar->AddItem(menu);
	menubar->AddItem(new BMenuItem("Help", new BMessage(MENU_HELP)));
	RecorderCount = 0;
	LogV->LogText->SetFontAndColor(0, 0, be_plain_font, B_FONT_ALL, &red);
	LogV->LogText->Insert("Welcome to BurnIt NOW! ");
	LogV->LogText->Insert(VERSION);
	LogV->LogText->Insert("\n ©2000 Johan Nilsson\n\n\n");

	CheckForDevices();
	Show();
}


void jpWindow::AWindow()
{
	AboutWin = new AboutWindow();
	AboutWin->Show();
}


uint64 jpWindow::GetVRCDSize()
{
	char command[1024];
	char buffer[1024], buf[1024];
	uint32 i;
	uint64 tempas = 0;
	if (IMAGE_TYPE == 0) {
		FILE* f1;
		sprintf(command, "mkisofs -print-size %s %s -gui -f -V \"%s\" -C %s \"%s\" 2>&1", DATA_STRING, BOOTSTRING, VOL_NAME, BURNITDEV->scsiid, BURN_DIR);
		f1 = popen(command, "r");
		while (!feof(f1) && !ferror(f1)) {
			buffer[0] = 0;
			fgets(buffer, 1024, f1);
			if (!strncmp("Total extents scheduled to be written = ", &buffer[0], 40)) {
				for (i = 0; i < strlen(&buffer[40]); i++) {
					if ((buffer[i + 40] != '\n') || (buffer[i + 41] != '\0'))
						buf[i] = buffer[i + 40];
				}
			}
		}
		pclose(f1);
		tempas = atol(buf) * 2048;
	} else if (IMAGE_TYPE == 1) {
		tempas = GetBFSSize();
	}
	return tempas;
}


void jpWindow::CalculateSize()
{
	BFile f1;
	char temp_char[1024];
	char what[100];
	uint32 angle_temp[100];
	uint32 tracks, i;
	off_t temp, total_audio, total, total_iso, total_vrcd;
	total_audio = total = total_iso = total_vrcd = 0;
	temp = 0;
	nrtracks = 0;
	if (BURN_TYPE == 0) {
		sprintf(what, "DataCD");
		total_vrcd = total = total_iso = total_audio = 0;
		MyStatus->SetBarColor(blue);
		if (LList->CountItems() > 0) {
			LeftListItem* item1 = (LeftListItem*)LList->ItemAt(0);
			if (item1->ficon == LList->ISOIcon) {
				f1.SetTo(&item1->fref, B_READ_ONLY);
				f1.GetSize(&temp);
				total_iso += temp;
				angle_temp[0] = temp / 1024 / 1024;
				nrtracks = 1;
			} else if (item1->ficon == LList->VRCDIcon) {
				nrtracks = 1;
				total_vrcd = temp = GetVRCDSize();
				angle_temp[0] = temp / 1024 / 1024;
			}

		}
	}

	else if (BURN_TYPE == 1) {
		sprintf(what, "AudioCD");
		total_vrcd = total = total_iso = total_audio = temp = 0;
		MyStatus->SetBarColor(green);
		if (LList->CountItems() > 0) {
			LeftListItem* item1 = (LeftListItem*)LList->ItemAt(0);
			LeftListItem* item2 = (LeftListItem*)LList->ItemAt(1);
			if (item1->ficon == LList->AudioIcon) {
				tracks = LList->CountItems();
				for (i = 0; i < tracks; i++) {
					item1 = (LeftListItem*)LList->ItemAt(i);
					f1.SetTo(&item1->fref, B_READ_ONLY);
					f1.GetSize(&temp);
					total_audio += temp;
					angle_temp[i] = temp / 1024 / 1024;
					nrtracks++;
				}
			} else if (LList->CountItems() > 1)
				if (item2->ficon == LList->AudioIcon) {
					tracks = LList->CountItems();
					for (i = 1; i < tracks; i++) {
						item1 = (LeftListItem*)LList->ItemAt(i);
						f1.SetTo(&item1->fref, B_READ_ONLY);
						f1.GetSize(&temp);
						total_audio += temp;
						angle_temp[i - 1] = temp / 1024 / 1024;
						nrtracks++;
					}
				}
		}
	} else if (BURN_TYPE == 2) {
		sprintf(what, "MixCD");
		total_vrcd = total = total_iso = total_audio = 0;
		MyStatus->SetBarColor(greenblue);
		if (LList->CountItems() > 0) {
			LeftListItem* item1 = (LeftListItem*)LList->ItemAt(0);
			LeftListItem* item2 = (LeftListItem*)LList->ItemAt(1);
			if (item1->ficon == LList->ISOIcon) {
				nrtracks = 1;
				f1.SetTo(&item1->fref, B_READ_ONLY);
				f1.GetSize(&temp);
				total_iso = temp;
				angle_temp[nrtracks - 1] = temp / 1024 / 1024;
			} else if (item1->ficon == LList->VRCDIcon) {
				nrtracks = 1;
				total_vrcd = temp = GetVRCDSize();
				angle_temp[nrtracks - 1] = temp / 1024 / 1024;

			}

			if (item1->ficon == LList->AudioIcon) {
				tracks = LList->CountItems();
				for (i = 0; i < tracks; i++) {
					item1 = (LeftListItem*)LList->ItemAt(i);
					f1.SetTo(&item1->fref, B_READ_ONLY);
					f1.GetSize(&temp);
					total_audio += temp;
					nrtracks++;
					angle_temp[nrtracks - 1] = temp / 1024 / 1024;

				}
			}

			else if (LList->CountItems() > 1)
				if (item2->ficon == LList->AudioIcon) {
					tracks = LList->CountItems();
					for (i = 1; i < tracks; i++) {
						item1 = (LeftListItem*)LList->ItemAt(i);
						f1.SetTo(&item1->fref, B_READ_ONLY);
						f1.GetSize(&temp);
						total_audio += temp;
						nrtracks++;
						angle_temp[nrtracks - 1] = temp / 1024 / 1024;
					}
				}
		}
	}

	total = total_audio + total_iso + total_vrcd;
	sprintf(temp_char, "%s - [%lld of 650 Mb]", what, total / 1024 / 1024);
	MyStatus->Reset();
	MyStatus->SetMaxValue(CDSIZE);
	MyStatus->Update((total / 1024 / 1024), temp_char);

	for (i = 0; i < nrtracks; i++) {
		if (i == 0)
			angles[i] = ((float)360 / (float)(total / 1024 / 1024)) * (float)angle_temp[i];
		else
			angles[i] = ((float)360 / (float)(total / 1024 / 1024)) * (float)angle_temp[i] + angles[i - 1];

	}


}


void jpWindow::InitBurnIT()
{
	const char* tr;
	char temp_char[1024];
	app_info info;
	BPath path;
	be_app->GetAppInfo(&info);
	entry_ref ref = info.ref;
	BEntry(&ref, true).GetPath(&path);
	path.GetParent(&path);
	BURNIT_PATH = new char[strlen(path.Path())+1];
	strcpy(BURNIT_PATH, path.Path());
	sprintf(temp_char, "%s/tmp", BURNIT_PATH);
	if (!BEntry(temp_char).Exists()) {
		BDirectory(BURNIT_PATH).CreateDirectory(temp_char, NULL);
	}
	IMAGE_NAME = new char[1024];
	sprintf(IMAGE_NAME, "%s/BurnIT.raw", temp_char);
	sprintf(temp_char, "%s/VRCD", temp_char);
	if (!BEntry(temp_char).Exists()) {
		BDirectory(temp_char).CreateDirectory(temp_char, NULL);
	}
	BURN_DIR = new char[strlen(temp_char)+1];
	strcpy(BURN_DIR, temp_char);

	// Load from pref file
	if (BurnITPrefs->FindString("ISOFILE_DIR", &tr) == B_OK) {
		strcpy(ISOFILE_DIR, tr);
	} else {
		strcpy(ISOFILE_DIR, "NONE");
	}
	if (BurnITPrefs->FindString("VOL_NAME", &tr) == B_OK) {
		strcpy(VOL_NAME, tr);
	} else {
		strcpy(VOL_NAME, "BurnIT");
	}
	if (BurnITPrefs->FindBool("VRCD", &VRCD) != B_OK)
		VRCD = false;

	if (BurnITPrefs->FindBool("ISOFILE", &ISOFILE) != B_OK)
		ISOFILE = false;

	if (BurnITPrefs->FindInt16("BURN_SPD", &BURN_SPD) != B_OK)
		BURN_SPD = 2;

	if (BurnITPrefs->FindInt16("BLANK_TYPE", &BLANK_TYPE) != B_OK)
		BLANK_TYPE = 1;

	if (BurnITPrefs->FindInt16("BLANK_SPD", &BLANK_SPD) != B_OK)
		BLANK_SPD = 1;

	if (BurnITPrefs->FindInt16("BURN_TYPE", &BURN_TYPE) != B_OK)
		BURN_TYPE = 0;

	if (BurnITPrefs->FindBool("ONTHEFLY", &ONTHEFLY) != B_OK)
		ONTHEFLY = false;

	if (BurnITPrefs->FindString("MULTISESSION", &tr) == B_OK) {
		strcpy(MULTISESSION, tr);
	} else {
		strcpy(MULTISESSION, " ");
	}
	if (BurnITPrefs->FindString("DUMMYMODE", &tr) == B_OK) {
		strcpy(DUMMYMODE, tr);
	} else {
		strcpy(DUMMYMODE, " ");
	}
	if (BurnITPrefs->FindString("EJECT", &tr) == B_OK) {
		strcpy(EJECT, tr);
	} else {
		strcpy(EJECT, " ");
	}
	if (BurnITPrefs->FindString("DATA_STRING", &tr) == B_OK) {
		strcpy(DATA_STRING, tr);
	} else {
		strcpy(DATA_STRING, " ");
	}
	if (BurnITPrefs->FindInt16("IMAGE_TYPE", &IMAGE_TYPE) != B_OK)
		IMAGE_TYPE = 0;

	if (BurnITPrefs->FindInt16("SCSI_DEV", &SCSI_DEV) != B_OK)
		SCSI_DEV = -1;

	if (BurnITPrefs->FindString("PAD", &tr) == B_OK) {
		strcpy(PAD, tr);
	} else {
		strcpy(PAD, "-pad");
	}

	if (BurnITPrefs->FindString("DAO", &tr) == B_OK) {
		strcpy(DAO, tr);
	}

	else {
		strcpy(DAO, " ");
	}

	if (BurnITPrefs->FindString("BURNPROOF", &tr) == B_OK) {
		strcpy(BURNPROOF, tr);
	}

	else {
		strcpy(BURNPROOF, " ");
	}


	if (BurnITPrefs->FindString("NOFIX", &tr) == B_OK) {
		strcpy(NOFIX, tr);
	} else {
		strcpy(NOFIX, " ");
	}

	if (BurnITPrefs->FindString("PREEMP", &tr) == B_OK) {
		strcpy(PREEMP, tr);
	} else {
		strcpy(PREEMP, " ");
	}
	if (BurnITPrefs->FindString("SWAB", &tr) == B_OK) {
		strcpy(SWAB, tr);
	} else {
		strcpy(SWAB, " ");
	}
	if (BurnITPrefs->FindRef("BOOTIMAGEREF", &BOOTIMAGEREF) == B_OK)
		if (BEntry(&BOOTIMAGEREF, true).Exists())
			if (BurnITPrefs->FindBool("BOOTABLE", &BOOTABLE) == B_OK) {}

	// End loading from pref file

	JUST_IMAGE = false;
	CDSIZE = 650;

}


void jpWindow::CheckForDevices()
{
	bool got_it;
	char command[2000];
	char buffer[1024], buf[512];
	FILE* f;
	int i, j;
	int msg;

	got_it = false;
	sprintf(command, "/boot/home/config/bin/cdrecord -scanbus");
	if (BEntry("/boot/home/config/bin/cdrecord").Exists() && BEntry("/boot/home/config/bin/mkisofs").Exists()) {
		if (RecorderCount < 100) {
			Lock();
			f = popen(command, "r");
			while (!feof(f) && !ferror(f)) {
				buffer[0] = 0;
				fgets(buffer, 1024, f);
				if (!strncmp(buffer, "scsibus", 7)) scsibus = true;
				for (i = 0; i < (int)strlen(buffer); i++) {
					if (buffer[i] >= '0' && buffer[i] <= '9') {
						if (buffer[i + 1] == ',' || buffer[i + 2] == ',') {
							got_it = true;
							break;
						}
					}
				}
				if (got_it) {
					if (buffer[i + 11] == '\'') {
						RecorderCount++;
						// scsiid
						memset((char*)&scsidevs[RecorderCount - 1].scsiid[0], 0, 7);
						strncpy(scsidevs[RecorderCount - 1].scsiid, &buffer[i], 6);

						// scsi vendor
						i += 12;
						for (j = 0; j < (int)strlen(buffer) - i; j ++) {
							if (buffer[i + j] == '\'')
								break;
						}
						memset((char*)&scsidevs[RecorderCount - 1].scsi_vendor[0], 0, 20);
						strncpy(scsidevs[RecorderCount - 1].scsi_vendor, &buffer[i], j);

						// scsi name
						i += j + 3;
						for (j = 0; j < (int)strlen(buffer) - i; j ++) {
							if (buffer[i + j] == '\'')
								break;
						}
						memset((char*)&scsidevs[RecorderCount - 1].scsi_name[0], 0, 50);
						strncpy(scsidevs[RecorderCount - 1].scsi_name, &buffer[i], j);
						msg = 'dev\0' | RecorderCount;

						strcpy(buf, scsidevs[RecorderCount - 1].scsi_vendor);
						strcat(buf, scsidevs[RecorderCount - 1].scsi_name);
						PrefsV->Recorders->AddItem(new BMenuItem(buf, new BMessage(msg)));
					}
				}
			}
			pclose(f);

			LogV->LogText->SetFontAndColor(0, 0, be_plain_font, B_FONT_ALL, &blue);
			sprintf(buf, "Found %d devices.\n\n", RecorderCount);
			LogV->LogText->Insert(buf);
			if (SCSI_DEV != -1) {
				BURNITDEV = &scsidevs[SCSI_DEV - 1];
				PrefsV->Recorders->ItemAt(SCSI_DEV - 1)->SetMarked(true);
			} else {
				BURNITDEV = NULL;
			}
			Unlock();
		}
	} else {
		Lock();
		LogV->LogText->SetFontAndColor(0, 0, be_plain_font, B_FONT_ALL, &blue);
		LogV->LogText->Insert("Cound not find cdrecord/mkisofs check so its in /boot/home/config/bin.\nInstall cdrecord and restart BurnIT NOW!");
		Unlock();
		BAlert* MyAlert = new BAlert("BurnIT", "Could not find cdrecord/mkisofs. You have to install them in /boot/home/config/bin", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_STOP_ALERT);
		MyAlert->Go();
		BurnV->SetButton(false);
		CDRWV->BlankButton->SetEnabled(false);
	}
}


int jpWindow::CheckMulti(char* str)
{
	FILE* f;
	int temp;
	char buf[1024];
	f = popen("/boot/home/config/bin/cdrecord -msinfo dev=9,1,0 2>&1", "r");
	while (!feof(f) && !ferror(f)) {
		buf[0] = 0;
		fgets(buf, 1024, f);
		if (buf[0] == '/') {
			temp = strlen(buf) - strlen("Cannot read first writable address\n");
			if (!strncmp(&buf[temp], "Cannot read first writable address", strlen("Cannot read first writable address"))) {
				return -1;
			}
			temp = strlen(buf) - strlen("Cannot read session offset\n");
			if (!strncmp(&buf[temp], "Cannot read session offset", strlen("Cannot read session offset"))) {
				return 0;
			}

		}
		if (buf[0] >= '0' && buf[0] <= '9') {
			strncpy(str, buf, strlen(buf) - 1);
			return 1;
		}
	}
}


void jpWindow::BurnNOW()
{
	char buf[1024];
	int temp;
	temp = 0;
	if (BURNITDEV == NULL) {
		MyTab->Select(5);
		BAlert* MyAlert = new BAlert("BurnIT", "You have to select device to be able to burn!", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		MyAlert->Go();
	} else {
		// DataCD
		if (BURN_TYPE == 0) {
			if (VRCD && !ISOFILE) {
				if (strcmp(BURN_DIR, "NONE")) {
					if (BurnV->OnTheFly->Value() == 1) {
						ONTHEFLY = true;
						BurnWithCDRecord();
					} else {
						if (BurnV->Multi->Value() == 1) {
							temp = CheckMulti(buf);
						}
						if (temp != -1) {
							MakeImageNOW(temp, buf);
						} else {
							BAlert* MyAlert = new BAlert("BurnIT", "Put a blank CD or a CD that you have burned multisession on before.", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
							MyAlert->Go();
						}
					}
				} else {
					BAlert* MyAlert = new BAlert("BurnIT", "This is a BUG! this shouldnt happen send me a mail and tell me exactly how you did and write that its \"BUG #13\"", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
					MyAlert->Go();
				}

			} else if (!VRCD && ISOFILE) {
				if (strcmp(ISOFILE_DIR, "NONE")) {
					strcpy(IMAGE_NAME, ISOFILE_DIR);
					BurnWithCDRecord();
				} else {
					BAlert* MyAlert = new BAlert("BurnIT", "You have to choose an ISO file!", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
					MyAlert->Go();
				}
			} else {
				BAlert* MyAlert = new BAlert("BurnIT", "You have to Add an \"Virtual CD Directory\" or an ISOfile to be able to burn DataCD:s!", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
				MyAlert->Go();
			}
		}
		// AudioCD
		else if (BURN_TYPE == 1) {
			BPath temp_path;
			int32 count;
			int32 i;
			count = LList->CountItems();
			for (i = 0; i < count; i++) {
				LeftListItem* item = (LeftListItem*)LList->ItemAt(i);
				if (item->isAudio) {
					BEntry(&item->fref).GetPath(&temp_path);
					sprintf(AUDIO_FILES, "%s \"%s\"", AUDIO_FILES , temp_path.Path());
				}
			}
			BurnWithCDRecord();
		}
		// MixCD
		if (BURN_TYPE == 2) {
			BPath temp_path;
			int32 count;
			int32 i;
			count = LList->CountItems();
			for (i = 0; i < count; i++) {
				LeftListItem* item = (LeftListItem*)LList->ItemAt(i);
				if (item->isAudio) {
					BEntry(&item->fref).GetPath(&temp_path);
					sprintf(AUDIO_FILES, "%s \"%s\"", AUDIO_FILES , temp_path.Path());
				}
			}
			if (VRCD && !ISOFILE) {
				if (strcmp(BURN_DIR, "NONE")) {
					if (BurnV->OnTheFly->Value() == 1) {
						ONTHEFLY = true;
						BurnWithCDRecord();
					} else {
						if (BurnV->Multi->Value() == 1) {
							temp = CheckMulti(buf);
						}
						if (temp != -1) {
							MakeImageNOW(temp, buf);
						} else {
							BAlert* MyAlert = new BAlert("BurnIT", "Put a blank CD or a CD that you have burned multisession on before.", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
							MyAlert->Go();
						}
					}
				} else {
					BAlert* MyAlert = new BAlert("BurnIT", "This is a BUG! this shouldnt happen send me a mail and tell me exactly how you did and write that its \"BUG #13\"", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
					MyAlert->Go();
				}

			} else if (!VRCD && ISOFILE) {
				if (strcmp(ISOFILE_DIR, "NONE")) {
					strcpy(IMAGE_NAME, ISOFILE_DIR);
					BurnWithCDRecord();
				} else {
					BAlert* MyAlert = new BAlert("BurnIT", "You have to choose an ISO file!", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
					MyAlert->Go();
				}
			} else {
				BAlert* MyAlert = new BAlert("BurnIT", "You have to add a Virtual CD Directory/ISOfile to be able to burn a MixCD!", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
				MyAlert->Go();
			}
		}

	}
}


void jpWindow::BlankNOW()
{
	int Result;
	if (BURNITDEV == NULL) {
		MyTab->Select(5);
		BAlert* MyAlert = new BAlert("BurnIT", "You have to select device to be able to blank!", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		MyAlert->Go();
	} else {
		BAlert* MyAlert = new BAlert("Put in a CDRW", "Put in a CDRW", "Oops forget it!", "Allready done.", NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
		Result = MyAlert->Go();
		if (Result) {
			CalculateSize();
			StatusWin = new StatusWindow(VOL_NAME);
			StatusWin->SetAngles(angles, 0);
			StatusWin->Show();

			SetButtons(false);
			char command[2000];
			sprintf(command, "/boot/home/config/bin/cdrecord dev=%s speed=%d blank=%s 2>&1", BURNITDEV->scsiid, BLANK_SPD, BlType[BLANK_TYPE]);
			Lock();
			resume_thread(Cntrl = spawn_thread(controller, "Blanking", 15, command));
			snooze(500000);
			resume_thread(OPBlank = spawn_thread(OutPutBlank, "OutPutBlank", 15, StatusWin));
			snooze(500000);
			Unlock();


		}

	}
}


void jpWindow::PutLog(char* string)
{
	Lock();
	LogV->LogText->SetFontAndColor(0, 0, be_plain_font, B_FONT_ALL, &red2);
	LogV->LogText->Insert("*******\n");
	LogV->LogText->Insert(string);
	LogV->LogText->Insert("\n*******\n");
	LogV->LogText->SetFontAndColor(0, 0, be_plain_font, B_FONT_ALL, &black);
	LogV->LogText->ScrollToOffset(LogV->LogText->CountLines() * 100);
	Unlock();
}


void jpWindow::MessageLog(char* string)
{
	Lock();
	LogV->LogText->SetFontAndColor(0, 0, be_plain_font, B_FONT_ALL, &green);
	LogV->LogText->Insert(string);
	LogV->LogText->Insert("\n");
	LogV->LogText->SetFontAndColor(0, 0, be_plain_font, B_FONT_ALL, &black);
	LogV->LogText->ScrollToOffset(LogV->LogText->CountLines() * 100);
	Unlock();
}


void jpWindow::MessageReceived(BMessage* message)
{
	entry_ref temp_ref;
	int temp;
	char buf[30];
	uint8 count;
	switch (message->what) {
		case MENU_HELP: {
				char tempas[1024];
				entry_ref tempas_ref;
				sprintf(tempas, "%s/BurnIT help.html", BURNIT_PATH);
				MessageLog(tempas);
				if (BEntry(tempas).Exists()) {
					BEntry(tempas).GetRef(&tempas_ref);
					be_roster->Launch(&tempas_ref);
				} else {
					BAlert* MyAlert = new BAlert("BurnIT", "Cant find help (BurnIT help.html)\n Reinstall BurnIT NOW! to fix the problem.", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
					MyAlert->Go();
				}
			}
			break;
		case CALCULATE_SIZE: {
				if (VRCD && !ISOFILE) {
					BAlert* MyAlert = new BAlert("BurnIT", "This can take a moment because you have a Virtual CD Directory\nSo dont kill BurnIT NOW! because its dont answer its just calculating the size!", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
					MyAlert->Go();
				}

				CalculateSize();
			}
			break;
		case BURN_WITH_CDRECORD: {
				BurnWithCDRecord();
			}
			break;
		case WRITE_TO_LOG: {
				char* temp_str;
				bool temp_bool;
				message->FindString("Text", (const char**)&temp_str);
				message->FindBool("Error", &temp_bool);
				if (temp_bool)
					PutLog(temp_str);
				else
					MessageLog(temp_str);
			}
			break;
		case SET_BUTTONS_TRUE:
			SetButtons(true);
			break;
		case DATA_VIRTUALCD:
			VRCD = true;
			ISOFILE = false;
			DataV->BootableCD->SetEnabled(true);
			if (DataV->BootableCD->Value() == 1) {
				DataV->ChooseBootImage->SetEnabled(true);
			}
			break;
		case DATA_ISOFILE:
			VRCD = false;
			ISOFILE = true;
			DataV->BootableCD->SetEnabled(false);
			DataV->ChooseBootImage->SetEnabled(false);
			break;
		case DATA_ISO9660:
			IMAGE_TYPE = 0;
			sprintf(DATA_STRING, " ");
			if (BURN_TYPE == 0 || BURN_TYPE == 2) {
				BurnV->OnTheFly->SetEnabled(false);
				DataV->BootableCD->SetEnabled(true);
				if (DataV->BootableCD->Value() == 1)
					DataV->ChooseBootImage->SetEnabled(false);
			}
			if (BURN_TYPE == 1)
				BurnV->Multi->SetEnabled(false);
			break;
		case DATA_BFS:
			IMAGE_TYPE = 1;
			DataV->BootableCD->SetEnabled(false);
			DataV->ChooseBootImage->SetEnabled(false);
			BurnV->OnTheFly->SetEnabled(false);
			BurnV->Multi->SetEnabled(false);
			break;
		case DATA_HFS:
			sprintf(DATA_STRING, "-hfs");
			IMAGE_TYPE = 0;
			if (BURN_TYPE == 0 || BURN_TYPE == 2) {
				BurnV->OnTheFly->SetEnabled(false);
				DataV->BootableCD->SetEnabled(true);
				if (DataV->BootableCD->Value() == 1)
					DataV->ChooseBootImage->SetEnabled(false);
			}
			if (BURN_TYPE == 1)
				BurnV->Multi->SetEnabled(false);
			break;
		case DATA_JOLIET:
			sprintf(DATA_STRING, "-l -D -J");
			IMAGE_TYPE = 0;
			if (BURN_TYPE == 0 || BURN_TYPE == 2) {
				BurnV->OnTheFly->SetEnabled(false);
				DataV->BootableCD->SetEnabled(true);
				if (DataV->BootableCD->Value() == 1)
					DataV->ChooseBootImage->SetEnabled(false);
			}
			if (BURN_TYPE == 1)
				BurnV->Multi->SetEnabled(false);
			break;
		case DATA_ROCK:
			sprintf(DATA_STRING, "-l -L -r");
			IMAGE_TYPE = 0;
			if (BURN_TYPE == 0 || BURN_TYPE == 2) {
				BurnV->OnTheFly->SetEnabled(false);
				DataV->BootableCD->SetEnabled(true);
				if (DataV->BootableCD->Value() == 1)
					DataV->ChooseBootImage->SetEnabled(false);
			}
			if (BURN_TYPE == 1)
				BurnV->Multi->SetEnabled(false);
			break;
		case DATA_WINDOWS:
			sprintf(DATA_STRING, "-D -l");
			IMAGE_TYPE = 0;
			if (BURN_TYPE == 0 || BURN_TYPE == 2) {
				BurnV->OnTheFly->SetEnabled(false);
				DataV->BootableCD->SetEnabled(true);
				if (DataV->BootableCD->Value() == 1)
					DataV->ChooseBootImage->SetEnabled(false);
			}
			if (BURN_TYPE == 1)
				BurnV->Multi->SetEnabled(false);
			break;
		case DATA_REALROCK:
			sprintf(DATA_STRING, "-l -L -R");
			IMAGE_TYPE = 0;
			if (BURN_TYPE == 0 || BURN_TYPE == 2) {
				BurnV->OnTheFly->SetEnabled(false);
				DataV->BootableCD->SetEnabled(true);
				if (DataV->BootableCD->Value() == 1)
					DataV->ChooseBootImage->SetEnabled(false);
			}
			if (BURN_TYPE == 1)
				BurnV->Multi->SetEnabled(false);
			break;


		case BURN_DATA_CD:
			BURN_TYPE = 0;
			if (DataV->BeOS->Value() != 1) {
				if (BurnV->OnTheFly->Value() != 1) {
					BurnV->Multi->SetEnabled(true);
				}
				BurnV->OnTheFly->SetEnabled(true);
				DataV->BootableCD->SetEnabled(true);
				if (DataV->BootableCD->Value() == 1) {
					DataV->ChooseBootImage->SetEnabled(true);
				}
			}
			break;
		case BURN_AUDIO_CD:
			BURN_TYPE = 1;
			BurnV->OnTheFly->SetEnabled(false);
			BurnV->Multi->SetEnabled(false);
			DataV->BootableCD->SetEnabled(false);
			DataV->ChooseBootImage->SetEnabled(false);
			break;
		case BURN_MIX_CD:
			BURN_TYPE = 2;
			if (DataV->BeOS->Value() != 1) {
				BurnV->OnTheFly->SetEnabled(true);
				BurnV->Multi->SetEnabled(false);
				DataV->BootableCD->SetEnabled(true);
				if (DataV->BootableCD->Value() == 1) {
					DataV->ChooseBootImage->SetEnabled(true);
				}
			}
			break;
		case AUDIO_PAD:
			if (AudioV->Pad->Value() == 1) {
				strcpy(PAD, "-pad");
			} else {
				strcpy(PAD, " ");
			}
			break;
		case AUDIO_SWAB:
			if (AudioV->Swab->Value() == 1) {
				strcpy(SWAB, "-swab");
			} else {
				strcpy(SWAB, " ");
			}
			break;
		case AUDIO_NOFIX:
			if (AudioV->Nofix->Value() == 1) {
				strcpy(NOFIX, "-nofix");
			} else {
				strcpy(NOFIX, " ");
			}
			break;
		case AUDIO_PREEMP:
			if (AudioV->Preemp->Value() == 1) {
				strcpy(PREEMP, "-preemp");
			} else {
				strcpy(PREEMP, " ");
			}
			break;

		case BURN_MULTI:
			if (BurnV->Multi->Value() == 1) {
				strcpy(MULTISESSION, "-multi");
			} else {
				strcpy(MULTISESSION, " ");
			}
			break;
		case BURN_DUMMY_MODE:
			if (BurnV->DummyMode->Value() == 1) {
				strcpy(DUMMYMODE, "-dummy");
			} else {
				strcpy(DUMMYMODE, " ");
			}
			break;
		case BURN_ONTHEFLY:
			if (BurnV->OnTheFly->Value() == 1) {
				BurnV->Multi->SetEnabled(false);
				ONTHEFLY = true;
			} else {
				if (BURN_TYPE == 0)
					BurnV->Multi->SetEnabled(true);
				ONTHEFLY = false;
			}
			break;
		case BURN_EJECT:
			if (BurnV->Eject->Value() == 1) {
				strcpy(EJECT, "-eject");
			} else {
				strcpy(EJECT, " ");
			}
			break;
		case MISC_DAO:
			if (PrefsV->DAOOption->Value() == 1) {
				strcpy(DAO, "-dao");
			} else {
				strcpy(DAO, " ");
			}

			break;
		case MISC_BURNPROOF:
			if (PrefsV->BurnProof->Value() == 1) {
				strcpy(BURNPROOF, "driveropts = burnproof");
			} else {
				strcpy(BURNPROOF, " ");
			}
			break;

		case MENU_FILE_ABOUT:
			AWindow();
			break;
		case OPEN_ISO_FILE:
			OpenISOPanel->Show();
			break;
		case MAKE_IMAGE:
			BurnNOW();
			break;
		case MAKE_AND_SAVE_IMAGE:
			if (VRCD && !ISOFILE) {
				SaveISOPanel->Show();
			} else {
				BAlert* MyAlert = new BAlert("BurnIT", "You have to Add an \"Virtual CD Directory\" able to make an ISOfile!", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
				MyAlert->Go();
			}
			break;
		case B_SAVE_REQUESTED: {
				entry_ref tempas_ref;
				const char* name;
				message->FindRef("directory", &tempas_ref);
				message->FindString("name", &name);
				sprintf(IMAGE_NAME, "%s/%s", BPath(&tempas_ref).Path(), name);
				JUST_IMAGE = true;
				MakeImageNOW(0, "");
			}
			break;
		case BLANK_IT_NOW:
			BlankNOW();
			break;
		case BLANK_FULL:
			BLANK_TYPE = 0;
			break;
		case BLANK_FAST:
			BLANK_TYPE = 1;
			break;
		case BLANK_SESSION:
			BLANK_TYPE = 2;
			break;
		case BLANK_TRACK:
			BLANK_TYPE = 3;
			break;
		case BLANK_TRACK_TAIL:
			BLANK_TYPE = 4;
			break;
		case BLANK_UNRES:
			BLANK_TYPE = 5;
			break;
		case BLANK_UNCLOSE:
			BLANK_TYPE = 6;
			break;

		case SPEED_CHANGE:
			temp = (int)BurnV->Speed->Value();
			BURN_SPD = (int)(temp + 1) * 2;
			sprintf(buf, "Burning Speed [%dx]", BURN_SPD);
			BurnV->Speed->SetLabel(buf);
			break;

		case BLANK_SPEED_CHANGE:
			temp = (int)CDRWV->BlankSpeed->Value();
			BLANK_SPD = (int)(temp + 1) * 2;
			sprintf(buf, "Blank Speed [%dx]", BLANK_SPD);
			CDRWV->BlankSpeed->SetLabel(buf);
			break;

		case PARENT_DIR:
			RList->ParentDir();
			break;
		case MAKE_DIR:
			RList->CreateDir();
			break;
		case MAKE_DIRECTORY:
			const char* temp_char;
			message->FindString("DirName", &temp_char);
			if (strcmp(temp_char, "boot.catalog")) {
				RList->MakeDir(temp_char);
			} else {
				BAlert* MyAlert = new BAlert("BurnIT", "You cant create a directory named boot.catalog\nits reserved for making bootable cd:s.", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
				MyAlert->Go();
			}
			break;
		case NEW_VRCD:
			if (!ISOFILE && !VRCD) {
				LList->AddItem(new LeftListItem(&temp_ref, "VRCD - []", LList->VRCDIcon, NULL), 0);
				ISOFILE = false;
				VRCD = true;
				NewVRCD->SetEnabled(false);
				AddISO->SetEnabled(false);
				MakeDir->SetEnabled(true);
				ParentDir->SetEnabled(true);

				VolumeNameWindow = new AskName(BRect(200, 200, 440, 240), "Volume name", VOLUME_NAME, VOL_NAME);
				VolumeNameWindow->Show();
				DataV->BootableCD->SetEnabled(true);
				if (DataV->BootableCD->Value() == 1) {
					DataV->ChooseBootImage->SetEnabled(true);
				}
			} else {
				BAlert* MyAlert = new BAlert("BurnIT", "You can only have one ISOfile/VirtualCD on a CD!", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
				MyAlert->Go();
			}
			break;
		case VOLUME_NAME: {
				message->FindString("DirName", &temp_char);
				strcpy(VOL_NAME, temp_char);
				LeftListItem* item;

				if ((item = (LeftListItem*)LList->ItemAt(0)) != NULL) {
					if (item->ficon == LList->VRCDIcon) {
						if (BOOTABLE)
							sprintf(item->fname, "VRCD(Boot) - [%s]", temp_char);
						else
							sprintf(item->fname, "VRCD - [%s]", temp_char);

						item->fname[27] = 0;
						item->fname[26] = ']';
						LList->InvalidateItem(0);
					}
				}
			}
			break;
		// BOOT
		case BOOT_CHECKED: {
				LeftListItem* item;
				if (DataV->BootableCD->Value() == 1) {
					DataV->BootableCD->SetValue(0);
					DataV->ChooseBootImage->SetEnabled(false);
					BOOTABLE = false;
					DataV->FilePanel->Show();

				} else {
					BOOTABLE = false;
					sprintf(BOOTSTRING, " ");
					DataV->ChooseBootImage->SetEnabled(false);
					if ((item = (LeftListItem*)LList->ItemAt(0)) != NULL) {
						if (item->ficon == LList->VRCDIcon) {
							sprintf(item->fname, "VRCD - [%s]", VOL_NAME);
							item->fname[27] = 0;
							item->fname[26] = ']';
							LList->InvalidateItem(0);
						}
					}

				}
			}
			break;
		case BOOT_FILE_PANEL:
			DataV->BootableCD->SetValue(0);
			DataV->ChooseBootImage->SetEnabled(false);
			BOOTABLE = false;
			DataV->FilePanel->Show();
			break;
		case BOOT_CHANGE_IMAGE_NAME: {
				LeftListItem* item;
				off_t size;
				entry_ref ref;
				message->FindRef("refs", &ref);
				if (BEntry(&ref, true).GetSize(&size) == B_OK)
					if (size < (2880 * 1024)) {
						BOOTIMAGEREF = ref;
						DataV->BootableCD->SetValue(1);
						DataV->ChooseBootImage->SetEnabled(true);
						BOOTABLE = true;
						if ((item = (LeftListItem*)LList->ItemAt(0)) != NULL) {
							if (item->ficon == LList->VRCDIcon) {
								sprintf(item->fname, "VRCD(Boot) - [%s]", VOL_NAME);
								item->fname[27] = 0;
								item->fname[26] = ']';
								LList->InvalidateItem(0);
							}
						}
					} else {
						sprintf(BOOTSTRING, " ");
						DataV->BootableCD->SetValue(0);
						DataV->ChooseBootImage->SetEnabled(false);
						BOOTABLE = false;
						BAlert* MyAlert = new BAlert("BurnIT", "The bootimage cant be bigger than 2.88 Mb!", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
						MyAlert->Go();
					}


			}
			break;
		case CHANGE_VOL_NAME: {
				VolumeNameWindow = new AskName(BRect(200, 200, 440, 240), "Volume name", VOLUME_NAME, VOL_NAME);
				VolumeNameWindow->Show();
			}
			break;
		default:
			BWindow::MessageReceived(message);
			break;

	}
	if ((message->what & 0xffffff00) == 'dev\0') {
		count = (uint8)(message->what & 0xff);
		SCSI_DEV = count;
		BURNITDEV = &scsidevs[count - 1];
	}
}


void jpWindow::SaveData()
{
	// Save all prefs!
	BurnITPrefs->SetBool("BOOTABLE", BOOTABLE);
	BurnITPrefs->SetRef("BOOTIMAGEREF", &BOOTIMAGEREF);
	BurnITPrefs->SetString("ISOFILE_DIR", ISOFILE_DIR);
	BurnITPrefs->SetString("VOL_NAME", VOL_NAME);
	BurnITPrefs->SetBool("VRCD", VRCD);
	BurnITPrefs->SetBool("ISOFILE", ISOFILE);
	BurnITPrefs->SetInt16("BURN_SPD", BURN_SPD);
	BurnITPrefs->SetInt16("BLANK_TYPE", BLANK_TYPE);
	BurnITPrefs->SetInt16("BLANK_SPD", BLANK_SPD);
	BurnITPrefs->SetInt16("BURN_TYPE", BURN_TYPE);
	BurnITPrefs->SetBool("ONTHEFLY", ONTHEFLY);
	BurnITPrefs->SetString("MULTISESSION", MULTISESSION);
	BurnITPrefs->SetString("DUMMYMODE", DUMMYMODE);
	BurnITPrefs->SetString("EJECT", EJECT);
	BurnITPrefs->SetString("DATA_STRING", DATA_STRING);
	BurnITPrefs->SetString("PAD", PAD);
	BurnITPrefs->SetString("DAO", DAO);
	BurnITPrefs->SetString("BURNPROOF", BURNPROOF);
	BurnITPrefs->SetString("SWAB", SWAB);
	BurnITPrefs->SetString("NOFIX", NOFIX);
	BurnITPrefs->SetString("PREEMP", PREEMP);
	BurnITPrefs->SetInt16("IMAGE_TYPE", IMAGE_TYPE);
	BurnITPrefs->SetInt16("SCSI_DEV", SCSI_DEV);

	// All saved
	delete BurnITPrefs;
}


bool jpWindow::QuitRequested()
{
	SaveData();
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}


void jpWindow::SetISOFile(char* string)
{
	entry_ref temp_ref;
	if (!VRCD && !ISOFILE) {
		strcpy(ISOFILE_DIR, string);
		BEntry(string).GetRef(&temp_ref);
		LList->AddItem(new LeftListItem(&temp_ref, temp_ref.name, LList->ISOIcon, NULL), 0);
		NewVRCD->SetEnabled(false);
		AddISO->SetEnabled(false);
		ISOFILE = true;
		VRCD = false;

	} else {
		BAlert* MyAlert = new BAlert("BurnIT", "You can only have one ISOfile/VirtualCD on a CD!", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
		MyAlert->Go();
	}
}


void jpWindow::SetProgDir(char* string)
{
}


void jpWindow::MakeImageNOW(int Multi, char* str)
{
	/*float temp1
	if ( Cntrl != 0 && OPMkImage != 0) {
		kill_thread(Cntrl);
		kill_thread(OPMkImage);
		system("kill -9 mkisofs");
		BurnV->BurnButton->SetLabel("Burn");
		OPMkImage = 0;
		Cntrl = 0;
		PutLog("User Aborted burning (Makeing Image)");
		StatusWin->Lock();
		StatusWin->MyStatus->SetBarColor(red);
		temp1 = MyStatus->CurrentValue();
		StatusWin->MyStatus->Reset();
		StatusWin->MyStatus->Update(temp1);
		StatusWin->Unlock();
		BEntry(IMAGE_NAME, true).Remove();
	} else {*/
	int Result;
	BAlert* MyAlert = new BAlert("Burn?", "Do you want to make the image?", "I changed my mind.", "Do it!", NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
	Result = MyAlert->Go();
	if (Result) {
		StatusWin = new StatusWindow(VOL_NAME);
		StatusWin->Show();

		StatusWin->Lock();
		StatusWin->MyStatus->Reset();
		StatusWin->MyStatus->SetBarColor(blue);
		StatusWin->Unlock();
		char command[2000];
		if (BOOTABLE) {
			MakeBootImage();
		}

		if (IMAGE_TYPE == 0) {
			if (Multi == 0) {
				sprintf(command, "mkisofs -o \"%s\" %s %s -gui -f -V \"%s\" \"%s\" 2>&1", IMAGE_NAME, DATA_STRING, BOOTSTRING, VOL_NAME, BURN_DIR);
			}
			if (Multi == 1) {
				sprintf(command, "mkisofs -o \"%s\" %s %s -gui -f -V \"%s\" -C %s -M %s \"%s\" 2>&1", IMAGE_NAME, DATA_STRING, BOOTSTRING, VOL_NAME, str, BURNITDEV->scsiid, BURN_DIR);
			}
			Lock();
			resume_thread(Cntrl = spawn_thread(controller, "MakeingIMAGE", 5, command));
			snooze(500000);
			resume_thread(OPMkImage = spawn_thread(OutPutMkImage, "OutPutMakeingIMAGE", 5, StatusWin));
			snooze(500000);
			BurnV->BurnButton->SetLabel("Cancel");
			Unlock();

		}
		if (IMAGE_TYPE == 1) { // BFS
			MessageLog("Begin making and copying to bfs file...");
			StatusWin->Lock();
			StatusWin->StatusSetText("Making BFS image this can take several minutes..");
			StatusWin->Unlock();
			MakeBFS();
			MountBFS();
			BFSDir = new BDirectory("/BurnITNOWTempMount");
			CopyFiles();
			delete BFSDir;
			UnmountBFS();
			MessageLog("Done with bfs!");
			if (!JUST_IMAGE)
				BurnWithCDRecord();
			else {
				sprintf(IMAGE_NAME, "%s/tmp/BurnIT.raw", BURNIT_PATH);
				StatusWin->Lock();
				StatusWin->Ready();
				StatusWin->Unlock();
			}
		}
	}
//}
}


void jpWindow::GetTsize(char* tsize)
{
	char buffer[1024];
	char temp_char[1024];
	FILE* f;
	sprintf(temp_char, "/boot/home/config/bin/mkisofs -print-size %s -f -V \"%s\" \"%s\" 2>&1", DATA_STRING, VOL_NAME, BURN_DIR);
	f = popen(temp_char, "r");
	memset(temp_char, 0, 1024);
	while (!feof(f) && !ferror(f)) {
		buffer[0] = 0;
		fgets(buffer, 1024, f);
		if (!strncmp(buffer, "Total extents scheduled to be written = ", 40)) {
			strncpy(temp_char, &buffer[40], strlen(buffer) - 41);
			sprintf(tsize, "%ss", temp_char);
		}
	}
	pclose(f);

}


void jpWindow::BurnWithCDRecord()
{
	char tsize[512];
	int Result;

	CalculateSize();

	BAlert* MyAlert = new BAlert("Put in a CD", "Put in a CDR/CDRW", "Oops forget it!", "Allready done.", NULL, B_WIDTH_AS_USUAL, B_WARNING_ALERT);
	Result = MyAlert->Go();
	if (Result) {
		if (ONTHEFLY || ISOFILE || (BURN_TYPE == 1)) {
			StatusWin = new StatusWindow(VOL_NAME);
			StatusWin->Show();
		}
		StatusWin->Lock();
		StatusWin->MyStatus->Reset();
		StatusWin->MyStatus->SetBarColor(blue);
		StatusWin->Unlock();

		SetButtons(false);
		char command[2000];
		if (BURN_TYPE == 0) {
			StatusWin->Lock();
			StatusWin->SetAngles(angles, 1);
			StatusWin->Unlock();

			if (ONTHEFLY && !ISOFILE && VRCD) {
				if (BOOTABLE) {
					MakeBootImage();
				}

				GetTsize(tsize);
				sprintf(command, "/boot/home/config/bin/mkisofs %s -quiet %s -f -V \"%s\" \"%s\" | /boot/home/config/bin/cdrecord dev=%s speed=%d %s tsize=%s %s -data %s %s -v -", DATA_STRING, BOOTSTRING, VOL_NAME, BURN_DIR, BURNITDEV->scsiid, BURN_SPD, BURNPROOF, tsize, DAO, DUMMYMODE, EJECT);
			} else {
				sprintf(command, "/boot/home/config/bin/cdrecord dev=%s speed=%d %s %s -data %s %s %s -v \"%s\"", BURNITDEV->scsiid, BURN_SPD, BURNPROOF, DAO, DUMMYMODE, EJECT, MULTISESSION, IMAGE_NAME);
			}
			Lock();
			resume_thread(Cntrl = spawn_thread(controller, "Burning", 15, command));
			snooze(500000);
			resume_thread(OPBurn = spawn_thread(OutPutBurn, "OutPutBurning", 15, StatusWin));
			snooze(500000);
			Unlock();
		} else if (BURN_TYPE == 1) {
			StatusWin->Lock();
			StatusWin->SetAngles(angles, nrtracks);
			StatusWin->Unlock();

			sprintf(command, "/boot/home/config/bin/cdrecord dev=%s speed=%d %s %s %s %s %s %s -audio %s %s -v %s", BURNITDEV->scsiid, BURN_SPD, BURNPROOF, DAO, PAD, PREEMP, SWAB, NOFIX, DUMMYMODE, EJECT, AUDIO_FILES);
			Lock();
			resume_thread(Cntrl = spawn_thread(controller, "Burning", 15, command));
			snooze(500000);
			resume_thread(OPBurn = spawn_thread(OutPutBurn, "OutPutBurning", 15, StatusWin));
			snooze(500000);
			Unlock();
		} else if (BURN_TYPE == 2) {
			StatusWin->Lock();
			StatusWin->SetAngles(angles, nrtracks);
			StatusWin->Unlock();

			if (ONTHEFLY && !ISOFILE && VRCD) {
				GetTsize(tsize);
				if (DataV->BootableCD->Value() == 1) {
					MakeBootImage();
				}
				sprintf(command, "/boot/home/config/bin/mkisofs %s %s -quiet -f -V \"%s\" \"%s\" | /boot/home/config/bin/cdrecord dev=%s speed=%d %s tsize=%s %s %s %s -v %s %s %s %s -data - -audio %s", DATA_STRING, BOOTSTRING, VOL_NAME, BURN_DIR, BURNITDEV->scsiid, BURN_SPD, BURNPROOF, tsize, DAO, DUMMYMODE, EJECT, PAD, PREEMP, SWAB, NOFIX, AUDIO_FILES);
				MessageLog(command);
			} else {
				sprintf(command, "/boot/home/config/bin/cdrecord dev=%s speed=%d %s %s %s %s %s %s %s %s -v -data \"%s\" -audio %s", BURNITDEV->scsiid, BURN_SPD, BURNPROOF, DAO, PAD, PREEMP, SWAB, NOFIX, DUMMYMODE, EJECT, IMAGE_NAME, AUDIO_FILES);
			}
			Lock();
			resume_thread(Cntrl = spawn_thread(controller, "Burning", 15, command));
			snooze(500000);
			resume_thread(OPBurn = spawn_thread(OutPutBurn, "OutPutBurning", 15, StatusWin));
			snooze(500000);
			Unlock();
		}

	}

}


void jpWindow::StatusSetMax(float t1)
{
}


void jpWindow::UpdateStatus(float delta, char* str)
{
	MyStatus->Update(delta - MyStatus->CurrentValue(), str);
}


void jpWindow::StatusSetText(char* str)
{
}


void jpWindow::StatusSetColor(rgb_color color)
{
}


void jpWindow::StatusUpdateReset()
{
}


void jpWindow::SetButtons(bool what)
{
	BurnV->SetButton(what);
	CDRWV->BlankButton->SetEnabled(what);
}


void jpWindow::MakeBootImage()
{
	char temp[1024];
	char temp2[1024];
	BPath path;
	BEntry entry(&BOOTIMAGEREF, true);
	if (entry.Exists()) {
		entry.GetPath(&path);
		sprintf(temp, "%s/boot.catalog", BURN_DIR);
		create_directory(temp, 0777);
		sprintf(BOOTSTRING, "-c boot.catalog/ -b \"boot.catalog/%s\"", path.Leaf());
		sprintf(temp, "%s/%s", temp, path.Leaf());
		sprintf(temp2, "dd if=/dev/zero of=\"%s\" bs=1024 count=2880", temp);
		system(temp2);
		sprintf(temp2, "dd if=\"%s\" of=\"%s\" conv=notrunc", path.Path(), temp);
		system(temp2);
	} else {
		sprintf(BOOTSTRING, " ");
		DataV->BootableCD->SetValue(0);
		DataV->ChooseBootImage->SetEnabled(false);
		BOOTABLE = false;
		BAlert* MyAlert = new BAlert("BurnIT", "The bootimage you choosen doesnt exist!\n BurnIT NOW! will burn this CD without a bootimage.", "Okey", NULL, NULL, B_WIDTH_AS_USUAL, B_INFO_ALERT);
		MyAlert->Go();
	}
}
