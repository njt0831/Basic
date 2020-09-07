#include "WindowConstants.hpp"
#include <opencv4/opencv2/core/core.hpp>
#include <unordered_map>
#include <limits.h>
#include <filesystem>
#include <sys/time.h>
#include <string>
#include <unistd.h>
#include <chrono>
#include <ctime>

extern "C"{
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xcursor/Xcursor.h>
}

class Basic {

	public:

		static Basic Create();
		void handleConfigureRequest(XConfigureRequestEvent ev);
		void handleMapRequest(XMapEvent ev);
		void handleButtonPress(XButtonPressedEvent ev);
		void handleButtonRelease(XButtonReleasedEvent ev);
		void handleUnmapNotify(XUnmapEvent ev);
		void handleMotionNotify(XPointerMovedEvent ev);
		void handleEnterNotify(XEnterWindowEvent ev);
		void handleLeaveNotify(XLeaveWindowEvent ev);
		void checkAction();
		XEvent event_;

	private:

			
	
