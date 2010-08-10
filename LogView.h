/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include <Be.h>
#include "const.h"
#include "IconLabel.h"


class LogView : public BView {
	public:
		LogView(BRect size);
		~LogView();
		IconLabel *LogLabel;
		BBox *BoxLog;
		BTextView *LogText;
		BScrollView *LogScroll;
	
};
