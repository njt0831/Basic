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

	// Initialize a log file object for debugging 
	f = fopen("/root/log.txt", "w");

}
