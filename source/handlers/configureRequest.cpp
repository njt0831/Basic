#include "Basic.hpp"


void Basic::handleConfigureRequest(XConfigureRequestEvent ev){


	tempWindowChanges.x = ev_.xconfigurerequest.x;
	tempWindowChanges.y = ev.xconfigurerequest.y;
	tempWindowChanges.width = ev.xconfigurerequest.width;
	tempWindowChanges.height = ev.xconfigurerequest.height;
	tempWindowChanges.border_width = ev.xconfigurerequest.border_width;
	tempWindowChanges.sibling = ev.xconfigurerequest.above;
	tempWindowChanges.stack_mode = ev.xconfigurerequest.detail;	

	if (clientFrame_.count(event_.xconfigurerequest.window)){

		frame = clientFrame_[event_.xconfigurerequest.window];
		XGetWindowAttributes(display_, frame, &winAtt);

		XMoveWindow(display_, event_.xconfigurerequest.window, 0, FRAME_TITLE_BAR_WIDTH);
		XResizeWindow(display_, event_.xconfigurerequest.window, winAtt.width, winAtt.height - FRAME_TITLE_BAR_WIDTH + FRAME_BORDER_WIDTH);

	}else{

		XConfigureWindow(display_, event_.xconfigurerequest.window, event_.xconfigurerequest.value_mask, &changes);
				
	}

}
