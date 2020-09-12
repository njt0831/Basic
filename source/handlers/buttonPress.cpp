#include "../Basic.hpp"

void Basic::handleButtonPress(XButtonPressedEvent ev){

	// If this is a close button, send a WM_DELETE_WINDOW event to the corresponding client 
	if(close_client_.count(ev.window)){

		tempEvent.type = ClientMessage;
		tempEvent.xclient.window = close_client_[ev.window];
		tempEvent.xclient.message_type = XInternAtom(display_, "WM_PROTOCOLS", true);
		tempEvent.xclient.format = 32;
		tempEvent.xclient.data.l[0] = XInternAtom(display_, "WM_DELETE_WINDOW", false);
		tempEvent.xclient.data.l[1] = CurrentTime;
		XSendEvent(display_, close_client_[ev.window], false, NoEventMask, &tempEvent);

	}else if (!(ev.window == root_) && ev.y < FRAME_TITLE_BAR_WIDTH){
		
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
