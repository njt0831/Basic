#include <X11/Xlib.h>
#include <stdio.h>
#include <unistd.h>
#include "/root/Basic/WindowConstants.hpp"
#include <string.h>
#include <math.h>
#include <iostream>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

int intFromString(char* str, int len){

	int result = 0;
	for(int c=0;c < len; c++){
		
		result = result + (str[c] - 48) * pow(10, len - c - 1);

	}

	return result;

}


int main(int argc, char** argv){

	if (argc < 2){
		
		return -1;

	}

	Display* display = XOpenDisplay(NULL);
	Window root = DefaultRootWindow(display);

	int xnew = 0;
	int ynew = 0;
	int wnew = 0;
	int hnew = 0;
	char full = 0;
	Window window = 0;

	for (unsigned int c = 1; c < argc; c++){

		if (!strcmp(argv[c], "-x")){

			xnew = intFromString(argv[c + 1], strlen(argv[c + 1]));
			c++;

		}else if(!strcmp(argv[c], "-y")){

			ynew = intFromString(argv[c + 1], strlen(argv[c + 1]));
			c++;

		}else if(!strcmp(argv[c], "-f")){
	
			if(!strcmp(argv[c + 1], "all")){

				full = 1;
			       
			}else if(!strcmp(argv[c + 1], "left")){

				full = 2;
			
			}else if(!strcmp(argv[c + 1], "right")){

				full = 3;
			
			}

			c++;
			
		}else if(!strcmp(argv[c], "-w")){
			
			wnew = intFromString(argv[c + 1], strlen(argv[c + 1]));
			c++;
			
		}else if(!strcmp(argv[c], "-h")){
			
			hnew = intFromString(argv[c + 1], strlen(argv[c + 1]));
			c++;

		}else if(!strcmp(argv[c], "-wn")){
			
			Window rootBack, parentBack;
			Window* subWindows;
			unsigned int subWindowCount;
			XQueryTree(display, root, &rootBack, &parentBack, &subWindows, &subWindowCount);
			XTextProperty textProp;
			XWindowAttributes winAtt;

			for (int i=0; i < subWindowCount; i++){
		
				XGetWindowAttributes(display, subWindows[i], &winAtt);	
				XGetWMName(display, subWindows[i], &textProp);

				if (winAtt.map_state != 2){
			
					continue;

				}

				if (!strcmp((char*) textProp.value, argv[c + 1])){

					window = subWindows[i];
					break;

				}
			}

		
			if (!window){

				// Didnt find the string. Put code here to print out all visible names
				return 1;

			}

			c++;
		}
	}	

	if (!window){
		
		int revertHolder;
		XGetInputFocus(display, &window, &revertHolder); 
	}
	
	Window rootBack, parentBack;
	Window* subWindows;
	unsigned int subWindowCount;
	XQueryTree(display, window, &rootBack, &parentBack, &subWindows, &subWindowCount);
	XSelectInput(display, window, StructureNotifyMask);


	if (full){

		if (full == 1){

			XResizeWindow(display, window, DISPLAY_WIDTH - FRAME_BORDER_WIDTH * 2, DISPLAY_HEIGHT - FRAME_BORDER_WIDTH * 2);
			XMoveWindow(display, window, 0, 0);

			for (unsigned int i=0;i < subWindowCount; i++){
		
				if (i){
			
					XMoveWindow(display, subWindows[i], (DISPLAY_WIDTH - FRAME_TITLE_BAR_WIDTH - 3) * i, 4);

				}else{

					XMoveWindow(display, subWindows[i], 0, FRAME_TITLE_BAR_WIDTH);
					XResizeWindow(display, subWindows[i], DISPLAY_WIDTH - FRAME_BORDER_WIDTH * 2, DISPLAY_HEIGHT - FRAME_TITLE_BAR_WIDTH - FRAME_BORDER_WIDTH * 2);
					
				}
			}
			
		}else if(full == 2){
			
			XResizeWindow(display, window, DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - FRAME_BORDER_WIDTH * 2);
			XMoveWindow(display, window, 0, 0);

			for (unsigned int i=0;i < subWindowCount; i++){
		
				if (i){
			
					XMoveWindow(display, subWindows[i], DISPLAY_WIDTH / 2 - FRAME_TITLE_BAR_WIDTH - 3, 4);

				}else{
					
					XMoveWindow(display, subWindows[i], 0, FRAME_TITLE_BAR_WIDTH);
					XResizeWindow(display, subWindows[i], DISPLAY_WIDTH / 2, DISPLAY_HEIGHT - FRAME_TITLE_BAR_WIDTH - FRAME_BORDER_WIDTH * 2);
					
				}
			}
		
		}else{

			XResizeWindow(display, window, DISPLAY_WIDTH / 2 - FRAME_BORDER_WIDTH * 2, DISPLAY_HEIGHT - FRAME_BORDER_WIDTH * 2);
			XMoveWindow(display, window, DISPLAY_WIDTH / 2, 0);

			for (unsigned int i=0;i < subWindowCount; i++){
		
				if (i){
			
					XMoveWindow(display, subWindows[i], DISPLAY_WIDTH / 2 - FRAME_TITLE_BAR_WIDTH - 3, 4);

				}else{
					XMoveWindow(display, subWindows[i], 0, FRAME_TITLE_BAR_WIDTH);
					XResizeWindow(display, subWindows[i], DISPLAY_WIDTH / 2 - FRAME_BORDER_WIDTH * 2, DISPLAY_HEIGHT - FRAME_TITLE_BAR_WIDTH - FRAME_BORDER_WIDTH * 2);
					

				}
			}
		
		}

	}else{

		XWindowAttributes winAtt;
		XGetWindowAttributes(display, window, &winAtt);

		if (!xnew){

			xnew = winAtt.x; 

		}

		if (!ynew){

			ynew = winAtt.y;
		
		}

		if (!wnew){

			wnew = winAtt.width;
		
		}

		if (!hnew){

			hnew = winAtt.height;

		}

		XResizeWindow(display, window, wnew, hnew);
		XMoveWindow(display, window, xnew, ynew);
	
		for (unsigned int i=0;i < subWindowCount; i++){
		
			if (i){
			
				XMoveWindow(display, subWindows[i], wnew - FRAME_TITLE_BAR_WIDTH - 3, 4);

			}else{

				XMoveWindow(display, subWindows[i], 0, FRAME_TITLE_BAR_WIDTH);
				XResizeWindow(display, subWindows[i], wnew, hnew  - FRAME_TITLE_BAR_WIDTH);

			}
		}
	
	}
	
	XSetInputFocus(display, window, RevertToParent, CurrentTime);
	XRaiseWindow(display, window);

	XEvent event_;

	while(1){
	
		XNextEvent(display, &event_);
		break;

	}
	
	return 0;

}
