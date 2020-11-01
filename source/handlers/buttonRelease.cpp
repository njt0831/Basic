#include "Basic.hpp"

void Basic::handleButtonRelease(XButtonReleasedEvent ev){	
	
	// If this is a frame
	if (frame_client_.count(ev.window)){
	
		// Get the last time we released a click
		diff = std::chrono::system_clock::now() - releaseTime;
		releaseTime = std::chrono::system_clock::now();

		// If this is a double click, then maximize the window
		if (diff.count() < .30){
			
			XGetWindowAttributes(display_, ev.window, &tempWindowAttributes);
			
			int monitor = getMonitor(tempWindowAttributes.x + ev.x, monitorOffsets, monitorWidths, monitorCount);
			int displayWidth = monitorWidths[monitor];
			int displayHeight = monitorHeights[monitor];
			int displayOffset = monitorOffsets[monitor];

			if (ev.y < FRAME_TITLE_BAR_WIDTH){
			       
				if ((tempWindowAttributes.width == displayWidth - (2 * FRAME_BORDER_WIDTH)) and (tempWindowAttributes.height == displayHeight - (2 * FRAME_BORDER_WIDTH) - TASKBAR_HEIGHT) and tempWindowAttributes.x == displayOffset and tempWindowAttributes.y == 0){
				
					XMoveWindow(display_, ev.window, displayOffset + 300, 150);
					XResizeWindow(display_, ev.window, 400, 400);
					XResizeWindow(display_, frame_client_[ev.window], 400 - (FRAME_BORDER_WIDTH), 400 - (2 * FRAME_BORDER_WIDTH));
					XMoveWindow(display_, frame_close_[ev.window], displayOffset + 400 - (FRAME_BORDER_WIDTH) - 25, 4);
					XMoveWindow(display_, client_minimize_[frame_client_[ev.window]], displayOffset + 400 - (FRAME_BORDER_WIDTH) - 50, 4);

				}else{

					XMoveWindow(display_, ev.window, displayOffset, 0);
					XResizeWindow(display_, ev.window, displayWidth - (2 * FRAME_BORDER_WIDTH), displayHeight - (2 * FRAME_BORDER_WIDTH) - TASKBAR_HEIGHT);
					XResizeWindow(display_, frame_client_[ev.window], displayWidth - (2 * FRAME_BORDER_WIDTH), displayHeight - (2 * FRAME_BORDER_WIDTH) - FRAME_TITLE_BAR_WIDTH - TASKBAR_HEIGHT);
					XMoveWindow(display_, frame_close_[ev.window], displayOffset + displayWidth - (2 * FRAME_BORDER_WIDTH) - 25, 4);
					XMoveWindow(display_, client_minimize_[frame_client_[ev.window]], displayOffset + displayWidth - (2 * FRAME_BORDER_WIDTH) - 50, 4);
					
				}
		
			}else if(resizeDown){

				fprintf(f, "No\n");

			}else if(resizeRight){

				XMoveWindow(display_, ev.window, displayOffset + displayWidth / 2, 0);
				XResizeWindow(display_, ev.window, displayWidth / 2 - (2 * FRAME_BORDER_WIDTH), displayHeight - (2 * FRAME_BORDER_WIDTH) - TASKBAR_HEIGHT);
				XResizeWindow(display_, frame_client_[ev.window], displayWidth / 2 - (2 * FRAME_BORDER_WIDTH), displayHeight - (2 * FRAME_BORDER_WIDTH) - FRAME_TITLE_BAR_WIDTH - TASKBAR_HEIGHT);
				XMoveWindow(display_, frame_close_[ev.window], displayOffset + displayWidth / 2 - (2 * FRAME_BORDER_WIDTH) - 25, 4);
				XMoveWindow(display_, client_minimize_[frame_client_[ev.window]], displayOffset + displayWidth / 2 - (2 * FRAME_BORDER_WIDTH) - 50, 4);

			}else if (resizeLeft){
				
				XMoveWindow(display_, ev.window, displayOffset, 0);
				XResizeWindow(display_, ev.window, displayWidth / 2 - (2 * FRAME_BORDER_WIDTH), displayHeight - (2 * FRAME_BORDER_WIDTH) - TASKBAR_HEIGHT);
				XResizeWindow(display_, frame_client_[ev.window], displayWidth / 2 - (2 * FRAME_BORDER_WIDTH), displayHeight - (2 * FRAME_BORDER_WIDTH) - FRAME_TITLE_BAR_WIDTH - TASKBAR_HEIGHT);
				XMoveWindow(display_, frame_close_[ev.window], displayOffset + displayWidth / 2 - (2 * FRAME_BORDER_WIDTH) - 25, 4);
				XMoveWindow(display_, client_minimize_[frame_client_[ev.window]], displayOffset + displayWidth / 2 - (2 * FRAME_BORDER_WIDTH) - 50, 4);
			
			}

		}
	
	}

	// Reset the grab state, user is no longer clicking anything
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
