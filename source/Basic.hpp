#include "./utils/windowConstants.hpp"
#include "./utils/windowUtilities.hpp"
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>
#include <unordered_map>
#include <limits.h>
#include <filesystem>
#include <sys/time.h>
#include <string>
#include <unistd.h>
#include <chrono>
#include <ctime>
#include <vector>

extern "C"{
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xcursor/Xcursor.h>
}

class Basic {

	public:
		
		// Constructor
		Basic();

		// Event Handlers
		void handleConfigureRequest(XConfigureRequestEvent ev);
		void handleMapRequest(XMapRequestEvent ev);
		void handleButtonPress(XButtonPressedEvent ev);
		void handleButtonRelease(XButtonReleasedEvent ev);
		void handleUnmapNotify(XUnmapEvent ev);
		void handleMotionNotify(XPointerMovedEvent ev);
		void handleEnterNotify(XEnterWindowEvent ev);
		void handleLeaveNotify(XLeaveWindowEvent ev);
		
		Display* display_;
		Window root_;

	private:	

		// Window maps to relate clients, frames, and frame action windows
		std::unordered_map<Window, Window> client_frame_;
		std::unordered_map<Window, Window> frame_client_;
		std::unordered_map<Window, Window> close_client_;
		std::unordered_map<Window, Window> frame_close_;
		std::unordered_map<Window, Window> minimize_client_;
		std::unordered_map<Window, Window> client_minimize_;

		// Temporary variables used during event handling
		XWindowChanges tempWindowChanges;
		Window tempWindow, tempWindowFrame, tempWindowClose, tempWindowMinimize;
		XWindowAttributes tempWindowAttributes;
		XSetWindowAttributes tempSetWindowAttributes;
		XTextProperty tempTextProperty;
		XClassHint tempClassHint;
		XWMHints *tempWMHints;
		Cursor cursor;
		XEvent tempEvent;
		
		// Frame hooking 
		Window hookWin;
		int hookXOffset;
		int hookYOffset;

		std::vector<std::string> backgrounds;

		// Log file object
		FILE *f;



};	
