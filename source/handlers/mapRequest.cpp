#include "Basic.hpp"

void Basic::handleMapRequest(XMapRequestEvent ev){

	if (client-frame_.count(ev.xmaprequest.window)){
			
		return;
				
	}

	XGetWMName(display_, ev.xmaprequest.window, &tempTextProperty);
	XGetClassHint(display_, ev.xmaprequest.window, &tempClassHint);
	XGetWindowAttributes(display_, event_.xmaprequest.window, &tempWindowAttributes);
				
	tempWMHints = XGetWMHints(display_, ev.xmaprequest.window);

	if (!tempWMHints){
		
		XMapWindow(display_, ev.xmaprequest.window);
		return;
	}
				
	
	// Fix this when it matters
	// Some clients wont want to be parented so they dont set their WM_NAME property			
	if (tempTextProperty.value == NULL){

		if (tempClassHint.res_name == NULL){

			tempClassHint.res_name = (char*) "";
					
		}else{
			// Just set any Null textprop to spotify

			unsigned char* fuckspotify = (unsigned char*) "spotify";
			tempTextProperty.value = (unsigned char*) fuckspotify;
			tempTextProperty.encoding = XInternAtom(display_, "WM_NAME", true);
			tempTextProperty.nitems = 8;
			tempTextProperty.format = 8;
			
		}
				
	}else if (!(strcmp((char*)tempTextProperty.value, "Mozilla Firefox"))){
				
		unsigned char* fuckspotify = (unsigned char*) "firefox";
		tempTextProperty.value = (unsigned char*) fuckspotify;
		tempTextProperty.encoding = XInternAtom(display_, "WM_NAME", true);
		tempTextProperty.nitems = 8;
		tempTextProperty.format = 8;
			
	}

	if (!(strcmp((char*)tempTextProperty.value, "Steam")) || !(strcmp((char*)tempTextProperty.value, "Rocket League"))){
					
		XMapWindow(display_, ev.xmaprequest.window);
		return;
	
	}
	
////////////////// Figure out how clients tell the window manager they dont want a frame so you can delete the shit above this //////////////////////////////

	if (tempWindowAttributes.x < 0 || tempWindowAttributes.y < 0 || tempWindowAttributes.y > 1080 or tempWindowAttributes.x > 1920){
					
		XMoveWindow(display_, ev.xmaprequest.window, 0, 0);
		tempWindowAttributes.x = 0; 
		tempWindowAttributes.y = 0;
	
	}

	tempWindowFrame = XCreateSimpleWindow(display_, root_, tempWindowAttributes.x, tempWindowAttributes.y, tempWindowAttributes.width, tempWindowAttributes.height + FRAME_TITLE_BAR_WIDTH, FRAME_BORDER_WIDTH, FRAME_BORDER_COLOR, FRAME_COLOR);
	tempWindowClose = XCreateSimpleWindow(display_, root_, 0, 0, 20, 12, 1, 0x181616, 0x363333);
	tempWindowMinimize = XCreateSimpleWindow(display_, root_, 0, 0, 20, 12, 1, 0x181616, 0x363333);	
	
	XSelectInput(display_, tempWindowFrame, SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);
	XSelectInput(display_, tempWindowClose, ButtonPressMask);		
	XSelectInput(display_, tempWindowMinimize, ButtonPressMask);
	
	XReparentWindow(display_, ev.xmaprequest.window, tempWindowFrame, 0, FRAME_TITLE_BAR_WIDTH);
	XReparentWindow(display_, tempWindowClose, tempWindowFrame, tempWindowAttributes.width - 25, 4);	
	XReparentWindow(display_, tempWindowMinimize, tempWindowFrame, winAtt.width - 50, 4);

	XMapWindow(display_, tempWindowFrame);
	XMapWindow(display_, tempWindowClose);
	XMapWindow(display_, tempWindowMinimize);
	XMapWindow(display_, ev.xmaprequest.window);
	XMoveWindow(display_, ev.xmaprequest.window, 0, FRAME_TITLE_BAR_WIDTH);
	
	LoadResource("/root/Basic/resources/utilities/close.png", tempWindowClose, 20, 12);
	// Load Minimize button resource here

	XSetWMName(display_, tempWindowFrame, &tempTextProperty);
	
	tempSetWindowAttributes.backing_store = 2;
	XChangeWindowAttributes(display_, tempWindowFrame, CWBackingStore, &tempSetWindowAttributes);
	XDrawString(display_, newFrame, XDefaultGC(display_, DefaultScreen(display_)), FRAME_BORDER_WIDTH, (int) FRAME_TITLE_BAR_WIDTH / 2, (char*) tempTextProperty.value, strlen((char*) tempTextProperty.value));

	client-frame_[ev.xmaprequest.window] = tempWindowFrame;
	frame-client_[tempWindowFrame] = ev.xmaprequest.window;
	close-client_[tempWindowClose] = ev.xmaprequest.window;
	frame-close_[tempWindowFrame] = tempWindowClose;
	minimize-client_[tempWindowMinimize] = ev.xmaprequest.window;
				
	// Without this you dont get button events when clicking clients. Necessitates an XAllowEvents in the button press loop to forward the events to the clients. Eventually will want to grab the keyboard as well to allow for the addition of general keyboard shortcuts.  
	XGrabButton(display_, AnyButton, AnyModifier, tempWindowFrame, false, ButtonPressMask | ButtonReleaseMask, GrabModeSync, GrabModeAsync, None, None);

}
