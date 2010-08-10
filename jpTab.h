/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#include <Tab.h>
#include <View.h>

class jpTab : public BTab {
	public:
		virtual void DrawTab(BView *owner,BRect frame, tab_position pos, bool full=true);
};
