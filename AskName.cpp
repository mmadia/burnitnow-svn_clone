#include "AskName.h"
#include "const.h"


AskName::AskName(BRect frame,char *title, uint32 mess, char *what) : BWindow(frame,"AskName",B_FLOATING_WINDOW_LOOK,B_NORMAL_WINDOW_FEEL,B_NOT_ZOOMABLE | B_NOT_RESIZABLE) {
	BRect r,r2;
	if(title != NULL)
		SetTitle(title);
	else
		SetTitle("BurnIT NOW!");
	
	message = mess;
	
	r = Bounds();
	Around = new BView(r,"Around",B_FOLLOW_NONE,B_WILL_DRAW);
	Around->SetViewColor(216,216,216,0);
	AddChild(Around);
	
	r = Around->Bounds();
	r.InsetBy(5.0,5.0);
	r.right -= 70;
	r.bottom = 23;
	Name = new BTextView(r,"Name_",r,B_FOLLOW_NONE,B_WILL_DRAW);
	Name->SetWordWrap(false);
	Name->DisallowChar('\n');
	if(message == VOLUME_NAME) {
		//Name->DisallowChar(' ');
	} 
	Around->AddChild(new BScrollView("NameScroll",Name,B_FOLLOW_NONE,0,false,false,B_FANCY_BORDER));
	r2 = Around->Bounds();
	r2.InsetBy(5.0,5.0);
	r2.left = r.right + 5;
	r2.top -=3;
	r2.bottom = 25;
	Okey = new jpButton(r2,"Okey!","Okey!",new BMessage('STxt'));
	Around->AddChild(Okey);
	
	Name->MakeFocus();
	if(what != NULL) {
		Name->Insert(what);
		Name->SelectAll();
	}
}
void AskName::SendText() {
	BMessage *temp_msg;
	temp_msg = new BMessage(message);
	temp_msg->AddString("DirName",Name->Text());
	be_app->PostMessage(temp_msg);
	Quit();
}
void AskName::MessageReceived(BMessage* msg) {
	switch(msg->what) {
		case 'STxt' :
			SendText();		
		break;
		default :
			BWindow::MessageReceived(msg);
	}
}