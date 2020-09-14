#include "windowConstants.hpp"
#include "windowUtilities.hpp"
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
#include <math.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xcursor/Xcursor.h>
#include <X11/extensions/shape.h>

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
		
		// Taskbar objects
		Window taskbar;
		Window taskButton;

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
		int x_back, y_back;
		unsigned int h_back, w_back, d_back, bw_back;
		Window root_back;

		// Frame hooking 
		Window hookWin;
		int hookXOffset;
		int hookYOffset;
		int hookWidth;
		int hookHeight;
		bool resizeRight;
		bool resizeLeft;
		bool resizeDown;
		bool resizeUp;

		std::vector<std::string> backgrounds;
		
		// Log file object
		FILE *f;

};	
