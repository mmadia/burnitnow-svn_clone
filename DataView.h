#include <Be.h>
#include "IconLabel.h"
#include "const.h"
#include "jpButton.h"


class DataView : public BView {
	public:
		DataView(BRect size);
		~DataView();
		
		jpButton *ChooseDir;
		jpButton *ChooseBootImage;
		char ChooseLabel[1024];
		BCheckBox *BootableCD;
		BFilePanel *FilePanel;

		IconLabel *FSLabel,*BootLabel;
		BBox *FSBox,*BootBox;
		BRadioButton *ISO9660,*Joliet,*Windows,*Mac,*BeOS,*Rock,*RealRock,*Own;
		jpButton *ChangeVolName;

};