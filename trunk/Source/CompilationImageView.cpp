/*
 * Copyright 2010, BurnItNow Team. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include "CompilationImageView.h"

#include <Box.h>
#include <Button.h>
#include <LayoutBuilder.h>
#include <StringView.h>


const float kControlPadding = 5;


CompilationImageView::CompilationImageView()
	:
	BView("Image File(ISO/CUE)", B_WILL_DRAW, new BGroupLayout(B_VERTICAL, kControlPadding))
{
	SetViewColor(ui_color(B_PANEL_BACKGROUND_COLOR));

	BBox* imageInfoBox = new BBox("ImageInfoBBox");
	imageInfoBox->SetLabel("Image Information");

	BLayoutBuilder::Group<>(dynamic_cast<BGroupLayout*>(GetLayout()))
		.SetInsets(kControlPadding, kControlPadding, kControlPadding, kControlPadding)
		.AddGroup(B_HORIZONTAL)
			.Add(new BStringView("ImageFileStringView", "/path/to/image.iso"))
			.Add(new BButton("ChooseImageButton", "Choose File", new BMessage()))
			.End()
		.AddGroup(B_HORIZONTAL)
			.Add(imageInfoBox)
			.End();
}
