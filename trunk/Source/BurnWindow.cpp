#include "BurnWindow.h"

#include <Alert.h>
#include <Application.h>
#include <LayoutBuilder.h>
#include <MenuItem.h>


const int32 kOpenHelpMessage = 'Help';
const int32 kOpenWebsiteMessage = 'Site';


#pragma mark --Constructor/Destructor--


BurnWindow::BurnWindow(BRect frame, const char* title)
	:
	BWindow(frame, title, B_TITLED_WINDOW, B_ASYNCHRONOUS_CONTROLS | B_QUIT_ON_WINDOW_CLOSE)
{

	BLayoutBuilder::Group<>(this, B_VERTICAL, 1.0f)
		.Add(_CreateMenuBar())
		.Add(_CreateToolBar())
		.Add(_CreateTabView())
		.Add(_CreateDiskUsageView());
}


#pragma mark --BWindow Overrides--


void BurnWindow::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case kOpenWebsiteMessage:
			_OpenWebSite();
			break;
		case kOpenHelpMessage:
			_OpenHelp();
			break;
		default:
			BWindow::MessageReceived(message);
	}
}


#pragma mark --Private Interface Builders--


BMenuBar* BurnWindow::_CreateMenuBar()
{
	BMenuBar* menuBar = new BMenuBar("GlobalMenuBar");

	BMenu* fileMenu = new BMenu("File");
	menuBar->AddItem(fileMenu);

	BMenuItem* aboutItem = new BMenuItem("About ...", new BMessage(B_ABOUT_REQUESTED));
	aboutItem->SetTarget(be_app);
	fileMenu->AddItem(aboutItem);
	fileMenu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED), 'Q'));

	BMenu* helpMenu = new BMenu("Help");
	menuBar->AddItem(helpMenu);

	helpMenu->AddItem(new BMenuItem("Usage Instructions", new BMessage(kOpenHelpMessage)));
	helpMenu->AddItem(new BMenuItem("Project Website", new BMessage(kOpenWebsiteMessage)));

	return menuBar;
}


BView* BurnWindow::_CreateToolBar()
{
	return new BView("GlobalToolBar", B_WILL_DRAW);
}


BView* BurnWindow::_CreateTabView()
{
	return new BView("GlobalTabView", B_WILL_DRAW);
}


BView* BurnWindow::_CreateDiskUsageView()
{
	return new BView("GlobalDiskUsageView", B_WILL_DRAW);
}


#pragma mark --Private Message Handlers--


void BurnWindow::_OpenWebSite()
{
	// TODO Ask BRoster to launch a browser for the project website
	(new BAlert("OpenWebSiteAlert", "Not Implemented Yet", "Ok"))->Go();
}

void BurnWindow::_OpenHelp()
{
	// TODO Ask BRoster to launch a browser for the local documentation
	(new BAlert("OpenHelpAlert", "Not Implemented Yet", "Ok"))->Go();
}
