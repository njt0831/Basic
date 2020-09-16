#include "Basic.hpp"

void Basic::handleButtonRelease(XButtonReleasedEvent ev){

	hookWin = 0;
	hookXOffset = 0;
	hookYOffset = 0;
	hookWidth = 0;
	hookHeight = 0;
	resizeRight = 0;
	resizeLeft = 0;
	resizeDown = 0;
	resizeUp = 0;	
	
	if (frame_client_.count(ev.window)){
	
		diff = std::chrono::system_clock::now() - releaseTime;
		releaseTime = std::chrono::system_clock::now();

		if (diff.count() < .30){

			
			XMoveWindow(display_, ev.window, 0, 0);
			XResizeWindow(display_, ev.window, DISPLAY_WIDTH - (2 * FRAME_BORDER_WIDTH), DISPLAY_HEIGHT - (2 * FRAME_BORDER_WIDTH));
			XResizeWindow(display_, frame_client_[ev.window], DISPLAY_WIDTH - (2 * FRAME_BORDER_WIDTH), DISPLAY_HEIGHT - (2 * FRAME_BORDER_WIDTH));
			XMoveWindow(display_, frame_close_[ev.window], DISPLAY_WIDTH - (2 * FRAME_BORDER_WIDTH) - 25, 4);
			XMoveWindow(display_, client_minimize_[frame_client_[ev.window]], DISPLAY_WIDTH - (2 * FRAME_BORDER_WIDTH) - 50, 4);

		}

	
	}

}
