#include "Basic.hpp"

void Basic::handleButtonPress(XButtonPressedEvent ev){

	if (dropdown && !(drop_index_.count(ev.window))){

		XDestroySubwindows(display_, dropdown);
		XDestroyWindow(display_, dropdown);
		dropdown = 0;
		
		for (unsigned int i=0; i < START_MENU_OPTION_COUNT; i++){
								
			drop_index_.clear();

		}	
		

	// If this is a close button, send a WM_DELETE_WINDOW event to the corresponding client 
	}else if (close_client_.count(ev.window)){

		tempEvent.type = ClientMessage;
		tempEvent.xclient.window = close_client_[ev.window];
		tempEvent.xclient.message_type = XInternAtom(display_, "WM_PROTOCOLS", true);
		tempEvent.xclient.format = 32;
		tempEvent.xclient.data.l[0] = XInternAtom(display_, "WM_DELETE_WINDOW", false);
		tempEvent.xclient.data.l[1] = CurrentTime;
		XSendEvent(display_, close_client_[ev.window], false, NoEventMask, &tempEvent);

	}else if (minimize_client_.count(ev.window)){
		
		tempWMHints = XGetWMHints(display_, minimize_client_[ev.window]);
					
		XGetGeometry(display_, tempWMHints->icon_pixmap, &root_back, &x_back, &y_back, &w_back, &h_back, &bw_back, &d_back);
		tempWindowFrame = XCreateSimpleWindow(display_, root_, x_back, y_back, w_back, h_back, 0, FRAME_BORDER_COLOR, FRAME_COLOR);
		
		XSetWindowBackgroundPixmap(display_, tempWindowFrame, tempWMHints->icon_pixmap);
		XMapWindow(display_, tempWindowFrame);
		XResizeWindow(display_, tempWindowFrame, 100, 100);
		XUnmapWindow(display_, minimize_client_[ev.window]);
	
	}else if (ev.window == taskButton){

		dropdown = XCreateSimpleWindow(display_, root_, 0, DISPLAY_HEIGHT - TASKBAR_HEIGHT - 4 - (START_MENU_OPTION_COUNT * START_MENU_HEIGHT), START_MENU_WIDTH, START_MENU_HEIGHT * START_MENU_OPTION_COUNT, 2, 0x000000, FRAME_COLOR);
		XMapWindow(display_, dropdown);
						
		for (unsigned int i=0;i < START_MENU_OPTION_COUNT; i++){
			
			tempWindowFrame = XCreateSimpleWindow(display_, root_, 0, DISPLAY_HEIGHT - TASKBAR_HEIGHT - 4 - (START_MENU_OPTION_COUNT * START_MENU_HEIGHT) + (i * START_MENU_HEIGHT), START_MENU_WIDTH, START_MENU_HEIGHT, 0, FRAME_BORDER_COLOR, FRAME_COLOR);
			XSelectInput(display_, tempWindowFrame, ButtonPressMask | EnterWindowMask | LeaveWindowMask);
			XReparentWindow(display_, tempWindowFrame, dropdown, 0, i * START_MENU_HEIGHT);
			XMapWindow(display_, tempWindowFrame);
			XDrawString(display_, tempWindowFrame, XDefaultGC(display_, DefaultScreen(display_)), 10, START_MENU_HEIGHT / 2 + 4, START_MENU_OPTIONS[i].c_str(), START_MENU_OPTIONS[i].length());	
			//dropWindows[i] = tempWindowFrame;
			drop_index_[tempWindowFrame] = i;
			
		}


	
	}else if (drop_index_.count(ev.window)){

		
		switch (drop_index_[ev.window]){

			case 0:

				loadResource(display_, root_, backgrounds[rand() % backgrounds.size()].c_str(), root_, 1920, 1080);
				break;

			case 1:

				std::system("cd /root;xterm -g 94x23+100+75 -bg black -fg blue&");
				break;

			case 2:

				break;

			case 3:

				fclose(f);
				XCloseDisplay(display_);
				break;
		
		}
	
	}else if(ev.window != root_){

		XSetInputFocus(display_, ev.window, RevertToParent, CurrentTime);
		XRaiseWindow(display_, ev.window);	
	
		if (frame_client_.count(ev.window)){	

			hookWin = ev.window;
			hookXOffset = ev.x;
			hookYOffset = ev.y;
			XGetWindowAttributes(display_, ev.window, &tempWindowAttributes);
			hookWidth = tempWindowAttributes.width;
			hookHeight = tempWindowAttributes.height;

			fprintf(f, "y is %i, ", ev.y);
			fprintf(f, "hookHeight is %i\n", hookHeight);

			if (ev.x < 4){resizeLeft = 1;}
			if (ev.x > hookWidth - 4){resizeRight = 1;}
			if (ev.y > hookHeight - 4){resizeDown = 1;}
			if (ev.y < 4){resizeUp = 1;}

		}

	}

	XAllowEvents(display_, ReplayPointer, CurrentTime);

}
