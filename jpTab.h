/*
 * Copyright 2000-2002, Johan Nilsson. All rights reserved.
 * Distributed under the terms of the MIT License.
 */
#ifndef _JPTAB_H_
#define _JPTAB_H_


#include <Tab.h>
#include <View.h>


class jpTab : public BTab {
public:
	virtual void DrawTab(BView* owner, BRect frame, tab_position pos, bool full = true);
};


#endif	// _JPTAB_H_
