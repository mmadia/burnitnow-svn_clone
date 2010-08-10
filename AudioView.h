#include <Be.h>
#include "IconLabel.h"
#include "const.h"


class AudioView : public BView {
	public:
		AudioView(BRect size);
		~AudioView();

		BCheckBox *Swab, *Nofix, *Preemp, *Pad;
		BBox *AdvOptions,*Info;
		IconLabel *AdvLabel;
		BTextView *InfoText;
		BScrollView *InfoScroll;	
};