#include "Basic.hpp"

void Basic::handleMotionNotify(XPointerMovedEvent ev){

	if (ev.window == hookWin){
	
		XGetWindowAttributes(display_, ev.window, &tempWindowAttributes);
		XMoveWindow(display_, ev.window, tempWindowAttributes.x + (ev.x - hookXOffset), tempWindowAttributes.y + (ev.y - hookYOffset));

	}

	XAllowEvents(display_, ReplayPointer, CurrentTime);

}
