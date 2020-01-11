extern "C"{
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xcursor/Xcursor.h>
}

#include "string.h"

int main(int argc, char** argv){

	printf("hi");

	if (argc < 3){
		return -1;
	}

	bool hide;
	char* winName;

	Display* display = XOpenDisplay(NULL);
	Window root = DefaultRootWindow(display);
	unsigned int i;

	for (i = 0; i < argc; i++){

		if (!strcmp(argv[i], "-m")){

			if (!strcmp(argv[i + 1], "hide")){

				hide = true;
			
			}else if(!strcmp(argv[i + 1], "show")){

				hide = false;
			}else{
				return -1;
			}

			i++;

		}else if(!strcmp(argv[i], "-w")){
	
			winName = argv[i + 1];
			i++;
			
		}
	}

	Window rootBack, parentBack;
	Window* subWindows;
	unsigned int subWindowCount;
	XQueryTree(display, root, &rootBack, &parentBack, &subWindows, &subWindowCount);
	XTextProperty textProp;

	char* list[subWindowCount]; 
	bool showFlag = false;

	for (i = 0; i < subWindowCount; i++){
		
		XGetWMName(display, subWindows[i], &textProp);
		
		if (textProp.value == NULL){
			
			list[i] = (char*)"whatthefuck";
			continue;
		
		}

		if (!strcmp((char*)textProp.value, winName)){

			// Attempting to unmap a frame
			// Current WM implementation ignores this map request
			// then this locks up waiting for an event it will never get

			// Can change this to correctly send unmap to the spotify client directly
			// Can change WM to unmap frames correctly, which will probably require a rewrite of unmap window notify catch

			if (hide){
				
				XUnmapWindow(display, subWindows[0]);
				showFlag = 1;
				break;

			}else{

				XMapWindow(display, subWindows[0]);
				showFlag = 1;
				break;

			}
		}	
		
		printf((char*)textProp.value);
		printf(" != ");
		printf(winName);
		printf("\n");
		list[i] = (char*)textProp.value;

	}

	if (!showFlag){

		printf("Current Window Names:\n");
	
		for (i = 0; i < subWindowCount; i++){
		
			printf(list[i]);
			printf("\n");
	
		}

		return -1;
	}
	

	XEvent event_;

	while(1){
	
		XNextEvent(display, &event_);
		break;

	}

	return -1;

}
	
