/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include <Be.h>
#include "const.h"
#include "IconLabel.h"

class BootView : public BView {
	public:
		BootView(BRect size);
		~BootView();
		BMenu *Recorders;
		BMenuField *RecordPop;
		BCheckBox *DAOOption;
		BBox *MiscOpt;
		IconLabel *MiscLabel;

};
