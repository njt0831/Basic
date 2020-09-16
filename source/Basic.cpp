#include "Basic.hpp"

Basic::Basic(){

	// Initialize display and root window
	display_ = XOpenDisplay(nullptr);
	root_ = DefaultRootWindow(display_);
	XSelectInput(display_, root_, SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

	// Initialize the cursor
	cursor = XCreateFontCursor(display_, 132);
	XDefineCursor(display_, root_, cursor);

	// Populate the backgrounds vector
	for(auto& file: std::filesystem::directory_iterator("/root/Basic/resources/backgrounds/")){
	
		backgrounds.push_back(file.path().string());
	
	}

	// Load a random desktop background
	loadResource(display_, root_, backgrounds[rand() % backgrounds.size()].c_str(), root_, 1920, 1080);

	// Recently added a tasbar, will likely refactor this somewhere else as I add functionality
	
	taskbar = XCreateSimpleWindow(display_, root_, 0, DISPLAY_HEIGHT - TASKBAR_HEIGHT, DISPLAY_WIDTH, TASKBAR_HEIGHT, 0, FRAME_COLOR, FRAME_COLOR); 
	XMapWindow(display_, taskbar);
	taskButton = XCreateSimpleWindow(display_, root_, 0, 0, TASKBAR_HEIGHT, TASKBAR_HEIGHT, 0, BUTTON_BACKGROUND_COLOR, 0); 
	XReparentWindow(display_, taskButton, taskbar, 0, 0);
	XSelectInput(display_, taskButton, ButtonPressMask);
	setCircular(display_, taskButton);
	XMapWindow(display_, taskButton);


	// Initialize the grab and dropdown state
	// Im open to debate about whether this should be contained in a method
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
	
	// Initialize a log file object for debugging 
	f = fopen("/root/log.txt", "w");

}
