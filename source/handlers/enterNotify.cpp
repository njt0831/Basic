#include "Basic.hpp"

void Basic::handleEnterNotify(XEnterWindowEvent ev){

	// If this is a start menu window
	if (drop_index_.count(ev.window)){

		// Highlight it
		XSetWindowBackground(display_, ev.window, START_MENU_HIGHLIGHT_COLOR);
		XClearWindow(display_, ev.window);
		XDrawString(display_, ev.window, XDefaultGC(display_, DefaultScreen(display_)), 10, START_MENU_HEIGHT / 2 + 4, START_MENU_OPTIONS[drop_index_[ev.window]].c_str(), START_MENU_OPTIONS[drop_index_[ev.window]].length());
		XFillRectangle(display_, ev.window, XDefaultGC(display_, DefaultScreen(display_)), 0, 0, 5, START_MENU_HEIGHT);
	
	}

}
