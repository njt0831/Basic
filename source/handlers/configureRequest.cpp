#include "Basic.hpp"

void Basic::handleConfigureRequest(XConfigureRequestEvent ev){	

	// If this is a framed window
	if (client_frame_.count(ev.window)){
		
		// Need to move the frame accordingly too
		tempWindow = client_frame_[ev.window];
		XGetWindowAttributes(display_, tempWindow, &tempWindowAttributes);
		
		XMoveWindow(display_, ev.window, 0, FRAME_TITLE_BAR_WIDTH);
		XResizeWindow(display_, ev.window, tempWindowAttributes.width, tempWindowAttributes.height - FRAME_TITLE_BAR_WIDTH + FRAME_BORDER_WIDTH);

	}else{

		// Just call XConfigureWindow
		tempWindowChanges.x = ev.x;
		tempWindowChanges.y = ev.y;
		tempWindowChanges.width = ev.width;
		tempWindowChanges.height = ev.height;
		tempWindowChanges.border_width = ev.border_width;
		tempWindowChanges.sibling = ev.above;
		tempWindowChanges.stack_mode = ev.detail;
		XConfigureWindow(display_, ev.window, ev.value_mask, &tempWindowChanges);
		
	}

}
