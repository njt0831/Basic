#include "Basic.hpp"

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

	}else if (minimize_client_.count(ev.window)){
		
		tempWMHints = XGetWMHints(display_, minimize_client_[ev.window]);
					
		XGetGeometry(display_, tempWMHints->icon_pixmap, &root_back, &x_back, &y_back, &w_back, &h_back, &bw_back, &d_back);
		tempWindowFrame = XCreateSimpleWindow(display_, root_, x_back, y_back, w_back, h_back, 0, FRAME_BORDER_COLOR, FRAME_COLOR);
		
		XSetWindowBackgroundPixmap(display_, tempWindowFrame, tempWMHints->icon_pixmap);
		XMapWindow(display_, tempWindowFrame);
		XResizeWindow(display_, tempWindowFrame, 100, 100);
		XUnmapWindow(display_, minimize_client_[ev.window]);
	
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
