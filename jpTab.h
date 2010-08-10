#include <Tab.h>
#include <View.h>

class jpTab : public BTab {
	public:
		virtual void DrawTab(BView *owner,BRect frame, tab_position pos, bool full=true);
};