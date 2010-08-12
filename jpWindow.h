/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _JPWINDOW_H_
#define _JPWINDOW_H_


#include <Box.h>
#include <Slider.h>
#include <StatusBar.h>
#include <TabView.h>
#include <Window.h>

#include "const.h"


// Forward declarations
class DataView;
class BurnView;
class AudioView;
class CopyCDView;
class PrefsView;
class IconLabel;
class LogView;
class CDRWView;
class RightList;
class LeftList;
class AskName;
class Prefs;
class StatusWindow;
class jpButton;
class AboutWindow;


class jpWindow : public BWindow {
public:
	jpWindow(BRect frame);
	virtual void InitBurnIt();
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
	Prefs* BurnItPrefs;

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
