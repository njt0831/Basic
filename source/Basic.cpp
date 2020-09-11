#include "Basic.hpp"

Basic::Basic(){

	display_ = XOpenDisplay(nullptr);
	root_ = DefaultRootWindow(display_);
	XSelectInput(display_, root_, SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

	cursor = XCreateFontCursor(display_, 132);
	XDefineCursor(display_, root_, cursor);

	f = fopen("/root/log.txt", "w");

	for(auto& file: std::filesystem::directory_iterator("/root/Basic/resources/backgrounds/")){
		
		backgrounds.push_back(file.path().string().c_str());

	}

	loadResource(display_, root_, backgrounds[0], root_, 1920, 1080);

}
