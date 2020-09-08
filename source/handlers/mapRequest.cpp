#include "Basic.hpp"

void Basic::handleMapRequest(XMapRequestEvent ev){

	if (clientFrame_.count(event_.xmaprequest.window)){
					
		break;
				
	}

	XGetWMName(display_, event_.xmaprequest.window, &textProp);
	XGetClassHint(display_, event_.xmaprequest.window, &winClass);
	XGetWindowAttributes(display_, event_.xmaprequest.window, &winAtt);
				
	hints = XGetWMHints(display_, event_.xmaprequest.window);
	

	if (!hints){
					
		XMapWindow(display_, event_.xmaprequest.window);
		break;
					
	}
					

	// Fix this when it matters
	// Some clients wont want to be parented so they dont set their WM_NAME property			
	if (textProp.value == NULL){

		if (winClass.res_name == NULL){

			winClass.res_name = (char*) "";
					
		}else{
			// Just set any Null textprop to spotify

			unsigned char* fuckspotify = (unsigned char*) "spotify";
			textProp.value = (unsigned char*) fuckspotify;
			textProp.encoding = XInternAtom(display_, "WM_NAME", true);
			textProp.nitems = 8;
			textProp.format = 8;
					
		}
				
	}else if (!(strcmp((char*)textProp.value, "Mozilla Firefox"))){
				
		unsigned char* fuckspotify = (unsigned char*) "firefox";
		textProp.value = (unsigned char*) fuckspotify;
		textProp.encoding = XInternAtom(display_, "WM_NAME", true);
		textProp.nitems = 8;
		textProp.format = 8;
					
	}

	if (!(strcmp((char*)textProp.value, "Steam")) || !(strcmp((char*)textProp.value, "Rocket League"))){
					
		XMapWindow(display_, event_.xmaprequest.window);
		break;
	
	}
				
////////////////// Figure out how clients tell the window manager they dont want a frame so you can delete the shit above this //////////////////////////////

	if (winAtt.x < 0 || winAtt.y < 0 || winAtt.y > 1080 or winAtt.x > 1920){
					
		XMoveWindow(display_, event_.xmaprequest.window, 0, 0);
		winAtt.x = 0; 
		winAtt.y = 0;
	
	}

	newFrame = XCreateSimpleWindow(display_, root_, winAtt.x, winAtt.y, winAtt.width, winAtt.height + FRAME_TITLE_BAR_WIDTH, FRAME_BORDER_WIDTH, FRAME_BORDER_COLOR, FRAME_COLOR);
	closeButton = XCreateSimpleWindow(display_, root_, 0, 0, 20, 12, 1, 0x181616, 0x363333);
	minimizeButton = XCreateSimpleWindow(display_, root_, 0, 0, 20, 12, 1, 0x181616, 0x363333);	
	
	XSelectInput(display_, newFrame, SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);
	XSelectInput(display_, closeButton, ButtonPressMask);		
	XSelectInput(display_, minimizeButton, ButtonPressMask);

	XReparentWindow(display_, event_.xmaprequest.window, newFrame, 0, FRAME_TITLE_BAR_WIDTH);
	XReparentWindow(display_, closeButton, newFrame, winAtt.width - 25, 4);	
	XReparentWindow(display_, minimizeButton, newFrame, winAtt.width - 50, 4);

	XMapWindow(display_, newFrame);	
	XMapWindow(display_, closeButton);
	XMapWindow(display_, minimizeButton);
	XMapWindow(display_, event_.xmaprequest.window);
	XMoveWindow(display_, event_.xmaprequest.window, 0, FRAME_TITLE_BAR_WIDTH); 

	LoadResource("/root/Basic/resources/utilities/close.png", closeButton, 20, 12);
	// Load Minimize button resource here

	XSetWMName(display_, newFrame, &textProp);

	setWinAtt.backing_store = 2;
	XChangeWindowAttributes(display_, newFrame, CWBackingStore, &setWinAtt);
	XDrawString(display_, newFrame, XDefaultGC(display_, DefaultScreen(display_)), FRAME_BORDER_WIDTH, (int) FRAME_TITLE_BAR_WIDTH / 2, (char*) textProp.value, strlen((char*) textProp.value));
	
	clientFrame_[event_.xmaprequest.window] = newFrame;
	frameClient_[newFrame] = event_.xmaprequest.window;
	killClient_[closeButton] = event_.xmaprequest.window;
	frameKill_[newFrame] = closeButton;
	minClient_[minimizeButton] = event_.xmaprequest.window;
				
	// Without this you dont get button events when clicking clients. Necessitates an XAllowEvents in the button press loop to forward the events to the clients. Eventually will want to grab the keyboard as well to allow for the addition of general keyboard shortcuts.  
	XGrabButton(display_, AnyButton, AnyModifier, newFrame, false, ButtonPressMask | ButtonReleaseMask, GrabModeSync, GrabModeAsync, None, None);

}
