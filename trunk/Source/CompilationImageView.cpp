/*
 * Copyright 2010, BurnItNow Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include "CompilationImageView.h"

#include <Box.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <Path.h>
#include <String.h>
#include <StringView.h>


const float kControlPadding = 5;

// Message Constants
const int32 kChooseImageMessage = 'Chus';


CompilationImageView::CompilationImageView()
	:
	BView("Image File(ISO/CUE)", B_WILL_DRAW, new BGroupLayout(B_VERTICAL, kControlPadding)),
	fOpenPanel(NULL),
	fImagePath(new BPath())
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BBox* imageInfoBox = new BBox("ImageInfoBBox");
	imageInfoBox->SetLabel("Image Information");

	BButton* chooseImageButton = new BButton("ChooseImageButton", "Choose File", new BMessage(kChooseImageMessage));
	chooseImageButton->SetTarget(this);

	BLayoutBuilder::Group<>(dynamic_cast<BGroupLayout*>(GetLayout()))
		.SetInsets(kControlPadding, kControlPadding, kControlPadding, kControlPadding)
		.AddGroup(B_HORIZONTAL)
			.Add(new BStringView("ImageFileStringView", "Image File: "))
			.Add(chooseImageButton)
			.End()
		.AddGroup(B_HORIZONTAL)
			.Add(imageInfoBox)
			.End();
}


CompilationImageView::~CompilationImageView()
{
	delete fImagePath;
}


#pragma mark -- BView Overrides --


void CompilationImageView::AttachedToWindow()
{
	BView::AttachedToWindow();

	BButton* chooseImageButton = dynamic_cast<BButton*>(FindView("ChooseImageButton"));
	if (chooseImageButton != NULL)
		chooseImageButton->SetTarget(this);
}


void CompilationImageView::MessageReceived(BMessage* message)
{
	switch (message->what) {
		case kChooseImageMessage:
			_ChooseImage();
			break;
		case B_REFS_RECEIVED:
			_OpenImage(message);
			break;
		default:
			BView::MessageReceived(message);
	}
}


#pragma mark -- Private Methods --


void CompilationImageView::_ChooseImage()
{
	// TODO Create a RefFilter for the panel?
	if (fOpenPanel == NULL)
		fOpenPanel = new BFilePanel(B_OPEN_PANEL, new BMessenger(this), NULL, B_FILE_NODE, false, NULL, NULL, true);

	fOpenPanel->Show();
}


void CompilationImageView::_OpenImage(BMessage* message)
{
	entry_ref imageRef;

	if (message->FindRef("refs", &imageRef) != B_OK)
		return;

	BStringView* imageFileStringView = dynamic_cast<BStringView*>(FindView("ImageFileStringView"));
	if (imageFileStringView == NULL)
		return;

	BString imageFileString("Image File: ");

	fImagePath->SetTo(&imageRef);
	imageFileString << fImagePath->Path();
	imageFileStringView->SetText(imageFileString.String());

	// TODO Verify that the file is a supported image type

	// TODO Spawn a job to collect image info
}
