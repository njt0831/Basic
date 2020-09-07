#include "Basic.hpp"
#include "X11/Xlib.h"


int main(){

	Basic basic = Basic::Create();
	basic.Initialize();

	while (1){
		
		XNextEvent(basic.display_, &basic.event_);

		switch (event_.type){

			case ConfigureRequest:
				
				basic.handleConfigureRequest();
				break;

			case MapRequest:

				basic.handleMapRequest();
				break;

			case UnmapNotify:

				basic.handleUnmapNotify();
				break;

			case ButtonPress:

				basic.handleButtonPress();
				break;

			case ButtonRelease:

				basic.handleButtonRelease();
				break;

			case MotionNotify:

				basic.handleMotionNotify();
				break;

			case EnterNotify:

				basic.handleEnterNotify();
				break;

			case LeaveNotify:

				basic.handleLeaveNotify();
				break;

	 		default:

				break;

		}

		basic.Action();
	
	}

}
