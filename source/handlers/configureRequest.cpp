#include "Basic.hpp"


void Basic::handleConfigureRequest(XConfigureRequestEvent ev){


	tempWindowChanges.x = ev_.xconfigurerequest.x;
	tempWindowChanges.y = ev.xconfigurerequest.y;
	tempWindowChanges.width = ev.xconfigurerequest.width;
	tempWindowChanges.height = ev.xconfigurerequest.height;
	tempWindowChanges.border_width = ev.xconfigurerequest.border_width;
	tempWindowChanges.sibling = ev.xconfigurerequest.above;
	tempWindowChanges.stack_mode = ev.xconfigurerequest.detail;	

	if (client-frame_.count(ev.xconfigurerequest.window)){
		
		tempWindow = client-frame_[ev_.xconfigurerequest.window];
		XGetWindowAttributes(display_, tempWindow, &tempWindowAttributes);
		
		XMoveWindow(display_, ev.xconfigurerequest.window, 0, FRAME_TITLE_BAR_WIDTH);
		XResizeWindow(display_, ev.xconfigurerequest.window, tempWindowAttributes.width, tempWindowAttributes.height - FRAME_TITLE_BAR_WIDTH + FRAME_BORDER_WIDTH);

	}else{

		XConfigureWindow(display_, ev.xconfigurerequest.window, ev.xconfigurerequest.value_mask, &tempWindowChanges);
		
	}

}
