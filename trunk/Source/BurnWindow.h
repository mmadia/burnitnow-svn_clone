#ifndef _BURNWINDOW_H_
#define _BURNWINDOW_H_


#include <MenuBar.h>
#include <View.h>
#include <Window.h>


class BurnWindow : public BWindow {
public:
	BurnWindow(BRect frame, const char* title);

	virtual void MessageReceived(BMessage* message);

private:
	BMenuBar* _CreateMenuBar();
	BView* _CreateToolBar();
	BView* _CreateTabView();
	BView* _CreateDiskUsageView();

	void _OpenWebSite();
	void _OpenHelp();
};


#endif	// _BURNWINDOW_H_
