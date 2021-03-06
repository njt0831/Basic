#include "Basic.hpp"

void Basic::handleMotionNotify(XPointerMovedEvent ev){

	// If this window is currently hooked
	if (ev.window == hookWin){
	
		// If the hook click grabbed the right side of the frame
		if (resizeRight){	
			
			// And the bottom (i.e. corner)
			if (resizeDown){
				
				XResizeWindow(display_, frame_client_[ev.window], hookWidth + (ev.x - hookXOffset), hookHeight + (ev.y - hookYOffset) - FRAME_TITLE_BAR_WIDTH + 2 * FRAME_BORDER_WIDTH);
				XMoveWindow(display_, client_minimize_[frame_client_[ev.window]], hookWidth + (ev.x - hookXOffset) - 50, 4);
				XMoveWindow(display_, frame_close_[ev.window], hookWidth + (ev.x - hookXOffset) - 25, 4);	
				XResizeWindow(display_, ev.window, hookWidth + (ev.x - hookXOffset), hookHeight + (ev.y - hookYOffset));
				
			}else{
				
				XResizeWindow(display_, frame_client_[ev.window], hookWidth + (ev.x - hookXOffset), hookHeight - FRAME_TITLE_BAR_WIDTH + 2 * FRAME_BORDER_WIDTH);
				XMoveWindow(display_, client_minimize_[frame_client_[ev.window]], hookWidth + (ev.x - hookXOffset) - 50, 4);
				XMoveWindow(display_, frame_close_[ev.window], hookWidth + (ev.x - hookXOffset) - 25, 4);	
				XResizeWindow(display_, ev.window, hookWidth + (ev.x - hookXOffset), hookHeight);
				
			}

		// Good luck getting here with the current hook implementation
		}else if (resizeDown){
			
			XResizeWindow(display_, frame_client_[ev.window], hookWidth, hookHeight + (ev.y - hookYOffset));
			XResizeWindow(display_, ev.window, hookWidth, hookHeight + (ev.y - hookYOffset));

		}else{

			XGetWindowAttributes(display_, ev.window, &tempWindowAttributes);
			XMoveWindow(display_, ev.window, tempWindowAttributes.x + (ev.x - hookXOffset), tempWindowAttributes.y + (ev.y - hookYOffset));
		}
	}

	XAllowEvents(display_, ReplayPointer, CurrentTime);

}
