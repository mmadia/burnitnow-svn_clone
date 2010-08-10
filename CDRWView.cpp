#include "CDRWView.h"
extern int16 BLANK_SPD;
CDRWView::CDRWView (BRect size) : BView(size,"CDRWView",B_FOLLOW_NONE,B_WILL_DRAW) {
	BRect r;
	char temp_char[100];
	SetViewColor(216,216,216,0);
	//CDRW BOX
	CDRWLabel = new IconLabel(BRect(0,0,19+be_bold_font->StringWidth(""),19),"","BMP:CDRW");
	CDRWLabel->SetViewColor(grey);
	r = Bounds();
	r.InsetBy(5.0,5.0);
	r.bottom = r.top + 70;
	CDRWBox = new BBox(r,"CDRWBox",B_FOLLOW_NONE,B_WILL_DRAW|B_FRAME_EVENTS,B_PLAIN_BORDER);
	CDRWBox->SetLabel(CDRWLabel);
	AddChild(CDRWBox);

//BlankMenu (CDRWBOX)
	r = CDRWBox->Bounds();
	r.InsetBy(5.0,5.0);
	r.top += 25;
	Blank = new BMenu("Select");
	Blank->SetLabelFromMarked(true);
	BlankPop = new BMenuField(r,"blank","Blank:",Blank);
	BlankPop->SetDivider(be_plain_font->StringWidth("Blank:  "));
	CDRWBox->AddChild(BlankPop);
	Blank->AddItem(new BMenuItem("Full",new BMessage(BLANK_FULL)));
	Blank->AddItem(new BMenuItem("Fast",new BMessage(BLANK_FAST)));
	Blank->AddItem(new BMenuItem("Session",new BMessage(BLANK_SESSION)));
	Blank->AddItem(new BMenuItem("Track",new BMessage(BLANK_TRACK)));
	Blank->AddItem(new BMenuItem("Track tail",new BMessage(BLANK_TRACK_TAIL)));
	Blank->AddItem(new BMenuItem("Unreserve",new BMessage(BLANK_UNRES)));
	Blank->AddItem(new BMenuItem("Unclose",new BMessage(BLANK_UNCLOSE)));
	

//BlankSpeed
	r = CDRWBox->Bounds();
	r.InsetBy(5.0,5.0);
	r.top += 15;
	r.left = 125;
	r.right = 250;
	sprintf(temp_char,"Blank Speed [%dx]",BLANK_SPD);
	BlankSpeed = new BSlider(r,"BlankSpeed",temp_char,new BMessage(BLANK_SPEED_CHANGE),0,5,B_BLOCK_THUMB,B_FOLLOW_NONE);
	BlankSpeed->SetHashMarks(B_HASH_MARKS_BOTTOM);
	BlankSpeed->SetHashMarkCount(6);
	BlankSpeed->SetValue((int32)(BLANK_SPD/2)-1);
	CDRWBox->AddChild(BlankSpeed);

//BlankButton
	r = CDRWBox->Bounds();
	r.InsetBy(5.0,5.0);
	r.top += 10;
	r.left = 300;
	r.right = 365;
	BlankButton = new jpButton(r,"BlankButton","Blank!",new BMessage(BLANK_IT_NOW));	
	CDRWBox->AddChild(BlankButton);

}
CDRWView::~CDRWView() {
}
