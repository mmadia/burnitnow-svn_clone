/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include <Be.h>
#include "const.h"
#include "IconLabel.h"

class PrefsView : public BView {
	public:
		PrefsView(BRect size);
		~PrefsView();
		BMenu *Recorders;
		BMenuField *RecordPop;
		BCheckBox *DAOOption,*BurnProof;
		BBox *MiscOpt;
		IconLabel *MiscLabel;

};
