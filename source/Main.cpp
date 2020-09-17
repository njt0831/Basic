#include "Basic.hpp"
#include "X11/Xlib.h"


// File is by design self explanatory

int main(){

	Basic* basic = new Basic();
	XEvent event_;
	
	std::system("xterm -g 94x23+100+75 -bg black -fg blue&");

	while (1){
		
		XNextEvent(basic->display_, &event_);

		switch (event_.type){

			case ConfigureRequest:
				
				basic->handleConfigureRequest(event_.xconfigurerequest);
				break;

			case MapRequest:

				basic->handleMapRequest(event_.xmaprequest);
				break;

			case UnmapNotify:

				basic->handleUnmapNotify(event_.xunmap);
				break;

			case ButtonPress:

				basic->handleButtonPress(event_.xbutton);
				break;

			case ButtonRelease:

				basic->handleButtonRelease(event_.xbutton);
				break;

			case MotionNotify:

				basic->handleMotionNotify(event_.xmotion);
				break;

			case EnterNotify:

				basic->handleEnterNotify(event_.xcrossing);
				break;

			case LeaveNotify:

				basic->handleLeaveNotify(event_.xcrossing);
				break;

	 		default:

				break;

		}
	
	}

}
