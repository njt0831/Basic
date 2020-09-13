#include "Basic.hpp"

void Basic::handleUnmapNotify(XUnmapEvent ev){

	if (!(client_frame_.count(ev.window)) || (ev.event == root_) || frame_client_.count(ev.window) || close_client_.count(ev.window)){return;}

	tempWindow = client_frame_[ev.window];

	if (hookWin == tempWindow){
		
		hookWin = 0;
		hookXOffset = 0;
		hookYOffset = 0;
		hookWidth = 0;
		hookHeight = 0;
		resizeRight = 0;
		resizeLeft = 0;
		resizeDown = 0;
		resizeUp = 0;

	}

	XUnmapWindow(display_, tempWindow);
	XDestroyWindow(display_, frame_close_[tempWindow]);
	XDestroyWindow(display_, tempWindow);

	client_frame_.erase(ev.window);
	frame_client_.erase(tempWindow);
	close_client_.erase(frame_close_[tempWindow]);
	minimize_client_.erase(client_minimize_[ev.window]);
	client_minimize_.erase(ev.window);
	frame_close_.erase(tempWindow);
	
}
