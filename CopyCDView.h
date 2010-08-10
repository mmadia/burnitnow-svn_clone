#include <Be.h>
#include "const.h"
#include "jpButton.h"


class CopyCDView : public BView {
	public:
		CopyCDView(BRect size, BWindow *twin);
		~CopyCDView();
		void MessageReceived(BMessage *message);
	private:
		BWindow *tempwin;
		jpButton *OpenDir;
};