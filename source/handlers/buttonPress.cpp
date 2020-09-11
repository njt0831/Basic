#include "../Basic.hpp"

void Basic::handleButtonPress(XButtonPressedEvent ev){

	
	if (!(ev.window == root_) && ev.y < FRAME_TITLE_BAR_WIDTH){
		
		hookWin = ev.window;
		hookXOffset = ev.x;
		hookYOffset = ev.y;

	}

	if (ev.button == Button3){
		
		fclose(f);
		XCloseDisplay(display_);
	}

	XAllowEvents(display_, ReplayPointer, CurrentTime);

}
