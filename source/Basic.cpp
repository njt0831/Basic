#include "Basic.hpp"

Basic::Basic(){

	// Initialize display and root window
	display_ = XOpenDisplay(nullptr);
	root_ = DefaultRootWindow(display_);
	XSelectInput(display_, root_, SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

	// Initialize the cursor
	cursor = XCreateFontCursor(display_, 132);
	XDefineCursor(display_, root_, cursor);

	// Populate the backgrounds vector. Background paths were stored at startup for convenient swapping.
	// Add customization later. Only need one background directory dump
	for(auto& file: std::filesystem::directory_iterator("/root/Basic/resources/backgrounds/")){
	
		backgrounds.push_back(file.path().string());
	
	}

	// Load a random desktop background
	loadResource(display_, root_, backgrounds[rand() % backgrounds.size()].c_str(), root_, 1920, 1080);

	// Initialize the grab and dropdown state
	// Should be contained in a method? Only ever done on startup and button release. 
	hookWin = 0;
	hookXOffset = 0;
	hookYOffset = 0;
	hookWidth = 0;
	hookHeight = 0;
	resizeRight = 0;
	resizeLeft = 0;
	resizeDown = 0;
	resizeUp = 0;
	dropdown = 0;

	// Get the size of all monitors that X detected on startup. As multi-monitor support is added this will likely need to change
	// For some reason, my attempts to store the return value of XRRGetMonitors in an XRRMonitorInfo* crashes the X server
	// Eventually will come back to solve this, but just calling it n times is a really tiny deal
	XRRGetMonitors(display_, root_, true, &monitorCount);

	monitorWidths = new int[monitorCount];
	monitorHeights = new int[monitorCount];
	monitorOffsets = new int[monitorCount];
	int sum = 0;

	for(int i=0;i<monitorCount;i++){
		
		monitorWidths[i] = XRRGetMonitors(display_, root_, true, &monitorCount)[i].width;
		monitorHeights[i] = XRRGetMonitors(display_, root_, true, &monitorCount)[i].height;
			
		monitorOffsets[i] = sum;
		sum = sum + monitorWidths[i];
		
		// Recently added the tasbar, refactor this somewhere else as functionality gets added and it makes sense
		taskbar = XCreateSimpleWindow(display_, root_, monitorOffsets[i], monitorHeights[i] - TASKBAR_HEIGHT, monitorWidths[i], TASKBAR_HEIGHT, 0, FRAME_COLOR, FRAME_COLOR); 
		XMapWindow(display_, taskbar);

		if (!i){
			
			taskButton = XCreateSimpleWindow(display_, root_, monitorOffsets[i], 0, TASKBAR_HEIGHT, TASKBAR_HEIGHT, 0, BUTTON_BACKGROUND_COLOR, 0); 
			XReparentWindow(display_, taskButton, taskbar, 0, 0);
			XSelectInput(display_, taskButton, ButtonPressMask);
			setCircular(display_, taskButton);
			XMapWindow(display_, taskButton);
			
		}
	}

	// Initialize a log file object for debugging
	// Importantly, it is only closed properly and subsequently logged to when the display is closed gracefully (crashed purposefully) via the start menu option
	// Not an intelligent debug method, but works for simple things
	f = fopen("/root/log.txt", "w");
	
}
