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
		
		// Constructor
		static Basic Basic();

		// Event Handlers
		void handleConfigureRequest(XConfigureRequestEvent ev);
		void handleMapRequest(XMapEvent ev);
		void handleButtonPress(XButtonPressedEvent ev);
		void handleButtonRelease(XButtonReleasedEvent ev);
		void handleUnmapNotify(XUnmapEvent ev);
		void handleMotionNotify(XPointerMovedEvent ev);
		void handleEnterNotify(XEnterWindowEvent ev);
		void handleLeaveNotify(XLeaveWindowEvent ev);
		
		// Something Something internal state update I dont remember
		void checkAction();
		XEvent event_;

	private:
		
		Display* display_;
		Window root_;

		// Window maps to relate clients, frames, and frame action windows
		client-frame_ = std::unordered_map<Window, Window>;



		// Temporary variables used during event handling
		XWindowChanges tempWindowChanges;
		

	
