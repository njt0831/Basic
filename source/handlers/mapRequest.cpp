#include "Basic.hpp"

void Basic::handleMapRequest(XMapRequestEvent ev){

	// If this window already has a frame dont need to do anything
	if (client_frame_.count(ev.window)){
		
		return;
		
	}
	
	// Useful things I will use someday
	XGetWMName(display_, ev.window, &tempTextProperty);
	XGetClassHint(display_, ev.window, &tempClassHint);	
	tempWMHints = XGetWMHints(display_, ev.window);

	XGetWindowAttributes(display_, ev.window, &tempWindowAttributes);

	// Does this window want any decorations?
	if (tempWindowAttributes.override_redirect){

		XMapWindow(display_, ev.window);
		return;

	}


	// Check if the window asking to be mapped is located off screen
	// If so just move to 0, 0 for now.
	if (tempWindowAttributes.x < 0 || tempWindowAttributes.y < 0 || tempWindowAttributes.y > 1080 or tempWindowAttributes.x > 1920){
					
		XMoveWindow(display_, ev.window, 0, 0);
		tempWindowAttributes.x = 0; 
		tempWindowAttributes.y = 0;
	
	}

	// Create the frame, close button, and minimize button
	tempWindowFrame = XCreateSimpleWindow(display_, root_, tempWindowAttributes.x, tempWindowAttributes.y, tempWindowAttributes.width, tempWindowAttributes.height + FRAME_TITLE_BAR_WIDTH, FRAME_BORDER_WIDTH, FRAME_BORDER_COLOR, FRAME_COLOR);
	tempWindowClose = XCreateSimpleWindow(display_, root_, 0, 0, FRAME_BUTTON_WIDTH, FRAME_BUTTON_HEIGHT, 1, BUTTON_BACKGROUND_COLOR, FRAME_COLOR);
	tempWindowMinimize = XCreateSimpleWindow(display_, root_, 0, 0, FRAME_BUTTON_WIDTH, FRAME_BUTTON_HEIGHT, 1, BUTTON_BACKGROUND_COLOR, FRAME_COLOR);	
	
	// Select inputs for the new windows
	XSelectInput(display_, tempWindowFrame, SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);
	XSelectInput(display_, tempWindowClose, ButtonPressMask);		
	XSelectInput(display_, tempWindowMinimize, ButtonPressMask);
	
	// Reparent the event window and close/minimize buttons to the new frame
	XReparentWindow(display_, ev.window, tempWindowFrame, 0, FRAME_TITLE_BAR_WIDTH);
	XReparentWindow(display_, tempWindowClose, tempWindowFrame, tempWindowAttributes.width - 25, 4);	
	XReparentWindow(display_, tempWindowMinimize, tempWindowFrame, tempWindowAttributes.width - 50, 4);

	// Map everything and move accordingly
	XMapWindow(display_, tempWindowFrame);
	XMapWindow(display_, tempWindowClose);
	XMapWindow(display_, tempWindowMinimize);
	XMapWindow(display_, ev.window);
	XMoveWindow(display_, ev.window, 0, FRAME_TITLE_BAR_WIDTH);
	
	// Load the close button icon
	loadResource(display_, root_, "/root/Basic/resources/utilities/close.png", tempWindowClose, 20, 12);
	loadResource(display_, root_, "/root/Basic/resources/utilities/underline.png", tempWindowMinimize, 20, 12);
	
	// Load Minimize button resource here

	// Set the WM Name property and draw the name onto the title bar
	XSetWMName(display_, tempWindowFrame, &tempTextProperty);
	tempSetWindowAttributes.backing_store = 2;
	XChangeWindowAttributes(display_, tempWindowFrame, CWBackingStore, &tempSetWindowAttributes);
	XDrawString(display_, tempWindowFrame, XDefaultGC(display_, DefaultScreen(display_)), FRAME_BORDER_WIDTH, (int) FRAME_TITLE_BAR_WIDTH / 2, (char*) tempTextProperty.value, strlen((char*) tempTextProperty.value));
	

	// Add all our new windows to the relationship maps
	client_frame_[ev.window] = tempWindowFrame;
	frame_client_[tempWindowFrame] = ev.window;
	close_client_[tempWindowClose] = ev.window;
	frame_close_[tempWindowFrame] = tempWindowClose;
	minimize_client_[tempWindowMinimize] = ev.window;
	client_minimize_[ev.window] = tempWindowMinimize;

	// Without this you dont get button events when clicking clients. Necessitates an XAllowEvents in the button press loop to forward the events to the clients. Eventually will want to grab the keyboard as well to allow for the addition of general keyboard shortcuts.  
	XGrabButton(display_, AnyButton, AnyModifier, tempWindowFrame, false, ButtonPressMask | ButtonReleaseMask, GrabModeSync, GrabModeAsync, None, None);

}
