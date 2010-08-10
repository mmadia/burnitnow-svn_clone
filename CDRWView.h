#include <Be.h>
#include "const.h"
#include "IconLabel.h"
#include "jpButton.h"


class CDRWView : public BView {
	public:
		CDRWView(BRect size);
		~CDRWView();
		BSlider *BlankSpeed;
		BBox *CDRWBox;
		IconLabel *CDRWLabel;
		BMenu *Blank;
		BMenuField *BlankPop;
		jpButton *BlankButton;

	
};