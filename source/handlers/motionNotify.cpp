#include "Basic.hpp"

void Basic::handleMotionNotify(XPointerMovedEvent ev){
	
	if (ev.window == hookWin){
	
		if (resizeRight){
			
			// This speeds it up but not nicely, the windows lag as they are resized and events are ignored
			XSync(display_, true);

			XResizeWindow(display_, frame_client_[ev.window], hookWidth + (ev.x - hookXOffset), hookHeight);
			XMoveWindow(display_, client_minimize_[frame_client_[ev.window]], hookWidth + (ev.x - hookXOffset) - 50, 4);
			XMoveWindow(display_, frame_close_[ev.window], hookWidth + (ev.x - hookXOffset) - 25, 4);	
			XResizeWindow(display_, ev.window, hookWidth + (ev.x - hookXOffset), hookHeight);

		}else{

			XGetWindowAttributes(display_, ev.window, &tempWindowAttributes);
			XMoveWindow(display_, ev.window, tempWindowAttributes.x + (ev.x - hookXOffset), tempWindowAttributes.y + (ev.y - hookYOffset));
		}
	}

	XAllowEvents(display_, ReplayPointer, CurrentTime);

}
