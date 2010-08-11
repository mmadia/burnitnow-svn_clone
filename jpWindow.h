/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _JPWINDOW_H_
#define _JPWINDOW_H_


#include <string.h>
#include <Be.h>
#include "DataView.h"
#include "BurnView.h"
#include "AudioView.h"
#include "CopyCDView.h"
#include "PrefsView.h"
#include "const.h"
#include "IconLabel.h"
#include "LogView.h"
#include "CDRWView.h"
#include "RightList.h"
#include "LeftList.h"
#include "AskName.h"
#include "Prefs.h"
#include "StatusWindow.h"
#include "jpButton.h"
#include "AboutWindow.h"


class jpWindow : public BWindow {
public:
	jpWindow(BRect frame);
	virtual void InitBurnIT();
	virtual bool QuitRequested();
	virtual void MessageReceived(BMessage* message);
	virtual void CheckForDevices();
	virtual void SetISOFile(char* string);
	virtual void SetProgDir(char* string);
	virtual void PutLog(char*);
	virtual void MessageLog(char*);
	virtual void MakeImageNOW(int, char*);
	virtual void BurnNOW();
	virtual void BlankNOW();
	virtual void BurnWithCDRecord();
	virtual void UpdateStatus(float delta, char* str);
	virtual void StatusSetMax(float t1);
	virtual void StatusSetText(char* str);
	virtual void StatusSetColor(rgb_color color);
	virtual void StatusUpdateReset();
	virtual void SetButtons(bool);
	virtual void AWindow();
	virtual int  CheckMulti(char* str);
	virtual void GetTsize(char* tsize);
	virtual void SaveData();
	virtual void MakeBootImage();
	virtual void CalculateSize();
	virtual uint64 GetVRCDSize();

	jpButton* ParentDir, *MakeDir, *NewVRCD, *AddISO, *CalcSize;
	RightList* RList;
	Prefs* BurnITPrefs;

	BSlider* Speed;
	BMenuBar* menubar;
	BBox* BoxAround;
	AskName* VolumeNameWindow;
	StatusWindow* StatusWin;
	AboutWindow* AboutWin;

	BFilePanel* OpenISOPanel, *SaveISOPanel;
	BStatusBar* MyStatus;
	BTabView* MyTab;
	BTab* MiscTab;

	BurnView* BurnV;
	DataView* DataV;
	AudioView* AudioV;
	PrefsView* PrefsV;
	LogView* LogV;
	CDRWView* CDRWV;


	LeftList* LList;


	struct cdrecorder scsidevs[100];
	int RecorderCount;
	struct cdrecorder* BURNITDEV;
};


#endif	// _JPWINDOW_H_
