#include "Basic.hpp"

void Basic::Mainloop(){

	Initialize();

	while (true){
		
		XNextEvent(display_, &event_);
 
		switch (event_.type){

			case ConfigureRequest:
				
				changes.x = event_.xconfigurerequest.x;
				changes.y = event_.xconfigurerequest.y;
				changes.width = event_.xconfigurerequest.width;
				changes.height = event_.xconfigurerequest.height;
				changes.border_width = event_.xconfigurerequest.border_width;
				changes.sibling = event_.xconfigurerequest.above;
				changes.stack_mode = event_.xconfigurerequest.detail;	

				XConfigureWindow(display_, event_.xconfigurerequest.window, event_.xconfigurerequest.value_mask, &changes);
				
	

				break;

			case MapRequest:
				
				if (clientFrame_.count(event_.xmaprequest.window)){
					
					break;
				
				}

				XGetWMName(display_, event_.xmaprequest.window, &textProp);
				XGetClassHint(display_, event_.xmaprequest.window, &winClass);
				XGetWindowAttributes(display_, event_.xmaprequest.window, &winAtt);

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
				
				}	
				
				//fprintf(f, "-------------------");
				//fprintf(f, (char*) textProp.value);
				//fprintf(f, "\n");
				//fprintf(f, winClass.res_name);
				//fprintf(f, "\n");
				//fprintf(f, winClass.res_class);
				//fprintf(f, "\n");
				//fprintf(f, "-------------------");

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
	
				XSelectInput(display_, newFrame, SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);
				XSelectInput(display_, closeButton, ButtonPressMask);	

				XMapWindow(display_, event_.xmaprequest.window);

				XReparentWindow(display_, event_.xmaprequest.window, newFrame, 0, FRAME_TITLE_BAR_WIDTH);
				XReparentWindow(display_, closeButton, newFrame, winAtt.width - 25, 4);	
	
				XMapWindow(display_, newFrame);	
				XMapWindow(display_, closeButton);	

				LoadResource("/root/Basic/resources/utilities/close.png", closeButton, 20, 12);
				
				XSetWMName(display_, newFrame, &textProp);

				setWinAtt.backing_store = 2;
				XChangeWindowAttributes(display_, newFrame, CWBackingStore, &setWinAtt);
				XDrawString(display_, newFrame, XDefaultGC(display_, DefaultScreen(display_)), FRAME_BORDER_WIDTH, (int) FRAME_TITLE_BAR_WIDTH / 2, (char*) textProp.value, strlen((char*) textProp.value));
	
				clientFrame_[event_.xmaprequest.window] = newFrame;
				frameClient_[newFrame] = event_.xmaprequest.window;
				killClient_[closeButton] = event_.xmaprequest.window;
				frameKill_[newFrame] = closeButton;
				
				// Why do I even implement the save set?
				//if (!saveSet_.count(xmaprequest.window)){

					//saveSet_.insert(event_.xmaprequest.window);
					//XAddToSaveSet(display_, event_.xmaprequest.window);

				//}
				XGrabButton(display_, AnyButton, AnyModifier, newFrame, false, ButtonPressMask | ButtonReleaseMask, GrabModeSync, GrabModeAsync, None, None);	

				break;

			case UnmapNotify:

				if (!(clientFrame_.count(event_.xunmap.window)) || (event_.xunmap.event == root_) || frameClient_.count(event_.xunmap.window) || killClient_.count(event_.xunmap.window)){break;}
				
				frame = clientFrame_[event_.xunmap.window];

				if (mouseHook && hookWin == frame){
				
					mouseHook = 0;
				
				}

				XUnmapWindow(display_, frame);
				XDestroySubwindows(display_, frame);
				XDestroyWindow(display_, frame);

				clientFrame_.erase(event_.xunmap.window);
				frameClient_.erase(frame);
				killClient_.erase(frameKill_[event_.xunmap.window]);
				frameKill_.erase(frame);

				break;

			case ButtonPress:
				
				if (killClient_.count(event_.xbutton.window)){

					sendEvent.type = ClientMessage;
					sendEvent.xclient.window = killClient_[event_.xbutton.window];
					sendEvent.xclient.message_type = XInternAtom(display_, "WM_PROTOCOLS", true);
					sendEvent.xclient.format = 32;
					sendEvent.xclient.data.l[0] = XInternAtom(display_, "WM_DELETE_WINDOW", false);
					sendEvent.xclient.data.l[1] = CurrentTime;
					XSendEvent(display_, killClient_[event_.xbutton.window], false, NoEventMask, &sendEvent);
				
			       		// fprintf(f, "Killing: %lld\n", event_.xbutton.window);
					// Deal with the case where a popup window prompts the user

				}else if ((event_.xbutton.window != root_) && (!dropIndex_.count(event_.xbutton.window))){
				
					if (clientFrame_.count(event_.xbutton.window)){
						
						frame = clientFrame_[event_.xbutton.window];
						XSetInputFocus(display_, frame, RevertToParent, CurrentTime);
						XRaiseWindow(display_, frame);	

					}else{
				
						XSetInputFocus(display_, event_.xbutton.window, RevertToParent, CurrentTime);
						XRaiseWindow(display_, event_.xbutton.window);
						XGetWindowAttributes(display_, event_.xbutton.window, &winAtt);
						SetGrabState(winAtt, event_.xbutton);
					}

				}else if (dropIndex_.count(event_.xbutton.window)){

					// Make this a switch moron

					if (dropIndex_[event_.xbutton.window] == 0){
						
						if (dropdown){
						
							XDestroySubwindows(display_, dropdown);
							XDestroyWindow(display_, dropdown);
							dropdown = 0;
							dropshowflag = 0;
							for (unsigned int i=0; i < DESKTOP_DROPDOWN_OPTION_COUNT; i++){
								
								dropIndex_.erase(dropWindows[i]);

							}
				
						}	


						NextRandomDesktopBackground();

					}else if (dropIndex_[event_.xbutton.window] == 1){
					
						std::system("xterm -g 94x23+100+75 -bg black -fg blue&");

					}else if(dropIndex_[event_.xbutton.window] == 2){

						int quickBar = XCreateSimpleWindow(display_, root_, 50, 700, 225, 65, 0, FRAME_BORDER_COLOR, FRAME_COLOR);
						XMapWindow(display_, quickBar);

						for(int i=0;i<1000;i++){
							
							XMoveWindow(display_, quickBar, 50 + i, 1050);

						}
						
					}else if (dropIndex_[event_.xbutton.window] == 3){

						CloseDisplay();
					
					}

				
				}else{

					if (event_.xbutton.button == Button3){
				
						if (dropdown){
						
							XDestroySubwindows(display_, dropdown);
							XDestroyWindow(display_, dropdown);
							dropdown = 0;
							dropshowflag = 0;
							for (unsigned int i=0; i < DESKTOP_DROPDOWN_OPTION_COUNT; i++){
								
								dropIndex_.erase(dropWindows[i]);

							}
				
						}

						dropdown = XCreateSimpleWindow(display_, root_, mousex, mousey, 200, DESKTOP_DROPDOWN_WIDTH * DESKTOP_DROPDOWN_OPTION_COUNT, 0, FRAME_BORDER_COLOR, FRAME_COLOR);
						XMapWindow(display_, dropdown);
						
						
						for (unsigned int i=0;i < DESKTOP_DROPDOWN_OPTION_COUNT; i++){
							
							temp = XCreateSimpleWindow(display_, root_, mousex, mousey + DESKTOP_DROPDOWN_WIDTH * i, 200, DESKTOP_DROPDOWN_WIDTH, 0, FRAME_BORDER_COLOR, FRAME_COLOR);
							XSelectInput(display_, temp, ButtonPressMask | ButtonReleaseMask | EnterWindowMask | LeaveWindowMask);
							XReparentWindow(display_, temp, dropdown, 0, i * DESKTOP_DROPDOWN_WIDTH);
							XMapWindow(display_, temp);
							XDrawString(display_, temp, XDefaultGC(display_, DefaultScreen(display_)), 10, DESKTOP_DROPDOWN_WIDTH / 2 + 4, DESKTOP_DROPDOWN_OPTIONS[i].c_str(), DESKTOP_DROPDOWN_OPTIONS[i].length());	
							dropWindows[i] = temp;
							dropIndex_[temp] = i;
						
						}
					}	

				}
				
				XAllowEvents(display_, ReplayPointer, CurrentTime);
				break;

			case ButtonRelease:
				
				ResetGrab();
				if (dropdown){

					if (dropshowflag){
						
						XDestroySubwindows(display_, dropdown);
						XDestroyWindow(display_, dropdown);
						dropdown = 0;
						dropshowflag = 0;
						for (unsigned int i=0; i < DESKTOP_DROPDOWN_OPTION_COUNT; i++){
							
							dropIndex_.erase(dropWindows[i]);
						}
					
					}else{

						dropshowflag = true;
					}
				
				}

				XAllowEvents(display_, ReplayPointer, CurrentTime);
				break;

			case MotionNotify:

				SetPointer(event_.xmotion.x_root, event_.xmotion.y_root);
				XAllowEvents(display_, ReplayPointer, CurrentTime);
				break;
			
			case EnterNotify:

				if (dropIndex_.count(event_.xcrossing.window)){

					XSetWindowBackground(display_, event_.xcrossing.window, DESKTOP_DROPDOWN_HIGHLIGHT_COLOR);
					XClearWindow(display_, event_.xcrossing.window);
					XDrawString(display_, event_.xcrossing.window, XDefaultGC(display_, DefaultScreen(display_)), 10, DESKTOP_DROPDOWN_WIDTH / 2 + 4, DESKTOP_DROPDOWN_OPTIONS[dropIndex_[event_.xcrossing.window]].c_str(), DESKTOP_DROPDOWN_OPTIONS[dropIndex_[event_.xcrossing.window]].length());
					XFillRectangle(display_, event_.xcrossing.window, XDefaultGC(display_, DefaultScreen(display_)), 0, 0, 5, DESKTOP_DROPDOWN_WIDTH);
					
				}

				break;

			case LeaveNotify:

				if (dropIndex_.count(event_.xcrossing.window)){

					XSetWindowBackground(display_, event_.xcrossing.window, FRAME_COLOR);
					XClearWindow(display_, event_.xcrossing.window);
					XDrawString(display_, event_.xcrossing.window, XDefaultGC(display_, DefaultScreen(display_)), 10, DESKTOP_DROPDOWN_WIDTH / 2 + 4, DESKTOP_DROPDOWN_OPTIONS[dropIndex_[event_.xcrossing.window]].c_str(), DESKTOP_DROPDOWN_OPTIONS[dropIndex_[event_.xcrossing.window]].length());

				}

				break;
			
			default:

				break;

		}

		Action();	
	}

}



