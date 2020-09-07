#include "WindowConstants.hpp"
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


extern "C"{
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>
#include <X11/Xcursor/Xcursor.h>
}


class Basic {

	public:

		static Basic Create();
		void Mainloop();

	private:
		
		Basic(Display* display);
		Display* display_;
		Window root_;
		XEvent event_;
		bool dropshowflag;

		std::filesystem::path backgroundPath;
		std::unordered_map<Window, Window> frameClient_;
		std::unordered_map<Window, Window> clientFrame_;
		std::unordered_map<Window, Window> killClient_;
		std::unordered_map<Window, Window> frameKill_;
		std::unordered_map<Window, int> dropIndex_;
		std::unordered_map<Window, Pixmap> winMap_;
		std::unordered_map<Window, Window> minClient_;

		Window dropWindows[DESKTOP_DROPDOWN_OPTION_COUNT];

		bool mouseHook, resizeLeft, resizeRight, resizeDown, resizeUp;
		unsigned int winHookx, winHooky, winHookWidth, winHookHeight, pHookx, pHooky, backgroundCount;
		int mousex, mousey, currentBackground;
		Window hookWin;
		std::chrono::time_point<std::chrono::system_clock> releaseTime;
		std::chrono::duration<double> diff;
		
		// TODO Figure out how to deal with this random shit. Probably shouldnt be member data
		cv::Mat img;
		XImage* xImage;
		XImage* xImageBackground;
		XWindowChanges changes;
		XWindowAttributes winAtt;
		XSetWindowAttributes setWinAtt;
		Cursor cursor;
		XEvent sendEvent;
	
		// TODO Refactor entire WM to prevent the need for this type of stupid bullshit
		int x_back, y_back;
		unsigned int h_back, w_back, d_back, bw_back;
		Window rootBack;

		//TODO figure out the actual number needed for these and name appropriately
		Window frame, newFrame, closeButton, eventsFrame, temp, dropdown, client, minimizeButton;
		Pixmap curMap, backgroundMap, intermediate;
		char* frontData;
		char* backData;
		char* prop_return;
		XClassHint winClass;
		XTextProperty textProp;
		XWMHints* hints;
		
		// Save set?  
		//std::Set saveSet_<Window>;

		FILE *f;

		void LoadResource(const char* resourcePath, Window window, unsigned int width, unsigned int height);
		void LoadBackgroundResource(const char* resourcePath, Window window, unsigned int width, unsigned int height);
		void CloseDisplay();
		void Initialize();
		void SetGrabState(XWindowAttributes winAtt, XButtonPressedEvent e);
		void DrawOpaqueWindow(Window drawFrame, double alphafore);
		void NextRandomDesktopBackground();
		void ResetGrab();
		void SetPointer(int mx, int my);
		void MaximizeWindow(Window window);
		void Action();
		
				
		//TODO actually implement error handling you fucking caveman
		
};
