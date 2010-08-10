#include <Be.h>
#include "const.h"
#include "IconLabel.h"
#include "jpButton.h"

class BurnView : public BView {
	public:
		BurnView(BRect size);
		~BurnView();
		void SetButton(bool);
		void SetLabel(char *);
//CDRW public
		 
//Burn public
		BSlider *BurnSpeed; 
		jpButton *BurnButton, *MakeImage;
		BCheckBox *Multi, *OnTheFly, *Eject, *DummyMode;
		BSlider *Speed;
		private:
//BurnBox
		BBox *BurnBox;
		IconLabel *BurnLabel,*DataCDLabel,*AudioCDLabel,*MixCDLabel,*bg;
		BRadioButton *RadioAudioCD,*RadioDataCD,*RadioMixCD;

};