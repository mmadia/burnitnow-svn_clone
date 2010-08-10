#include "AboutWindow.h"
#include "const.h"
#define  ABOUT_STRING "BurnIT NOW! beta 2"
#define ABOUT_STRING2 "©2000 Johan Nilsson"

BBitmap* GetBitmapResource(type_code type,const char* name);

AboutView::AboutView(BRect r, char *title) : BView(r,title,B_FOLLOW_NONE,B_WILL_DRAW)
{
	f1 = new BFont(be_plain_font);
	SetFont(f1);
	burnbitmap = GetBitmapResource('BBMP',"BMP:BACKGROUND");
	burnproof = GetBitmapResource('BBMP',"BMP:BURNPROOF");
	cdrecord = GetBitmapResource('BBMP',"BMP:CDRECORD");  
}
AboutView::~AboutView()
{
	delete f1;
	delete burnbitmap;
	delete burnproof;
	delete cdrecord;
}

void AboutView::Draw(BRect updateRect) {
	SetLowColor(grey);
	SetHighColor(grey);
	BRect r;
	BPoint p1;
	r = Bounds();
	FillRect(r);
	p1.x = (r.right/2)-32;
	p1.y = r.top + 10;
	DrawBitmap(burnbitmap,p1);
	f1->SetSize(15);
	f1->SetFace(B_BOLD_FACE);
	SetFont(f1);
	p1.x = (r.right/2)-(f1->StringWidth(ABOUT_STRING)/2);
	p1.y = r.top + 20;
	MovePenTo(p1);
	SetHighColor(0,0,0);
	DrawString(ABOUT_STRING);
	f1->SetSize(10);
	f1->SetFace(B_REGULAR_FACE);
	SetFont(f1);
	p1.x = (r.right/2)-(f1->StringWidth(ABOUT_STRING2)/2);
	p1.y += 15;
	MovePenTo(p1);
	SetHighColor(0,0,0);
	DrawString(ABOUT_STRING2);
	p1.x = r.left + 100;
	p1.y = r.top + 85;
	DrawBitmap(burnproof,p1);
	
	p1.x = r.left + 15;
	p1.y = r.top + 70;
	DrawBitmap(cdrecord,p1);

}
//B_BOLD_FACE
//B_REGULAR_FACE

AboutWindow::AboutWindow() : BWindow(BRect(250,200,460,350),"AboutWindow",B_MODAL_WINDOW_LOOK,B_MODAL_APP_WINDOW_FEEL,B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_NOT_CLOSABLE)
{
	BRect r;
	SetTitle("About");
	
	r = Bounds();
	Around = new BView(r,"Around",B_FOLLOW_ALL_SIDES,B_WILL_DRAW);
	Around->SetViewColor(216,216,216,0);
	AddChild(Around);

	r = Around->Bounds();
	r.InsetBy(5.0,5.0);
	r.top = r.bottom - 20;
	r.left = r.right - 35;
	Close = new jpButton(r,"close","Close",new BMessage('ClWi'));
	Around->AddChild(Close);
	r = Around->Bounds();
	r.InsetBy(5.0,5.0);
	r.bottom = r.bottom - 25;
	AView = new AboutView(r,"AboutView");
	Around->AddChild(AView);
}
void AboutWindow::MessageReceived(BMessage* msg)
{
	switch(msg->what)
	{
		case 'ClWi':
		{
			Quit();
		}
		break;
	}
}
