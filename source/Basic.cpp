#include "Basic.hpp"

Basic::Basic(){

	display_ = XOpenDisplay(nullptr);
	root_ = DefaultRootWindow(display_);
	XSelectInput(display_, root_, SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);

	cursor = XCreateFontCursor(display_, 132);
	XDefineCursor(display_, root_, cursor);
	
	f = fopen("/root/log.txt", "w");

}
