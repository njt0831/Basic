#include "Basic.hpp"

void Basic::handleLeaveNotify(XLeaveWindowEvent ev){


	if (drop_index_.count(ev.window)){

		XSetWindowBackground(display_, ev.window, FRAME_COLOR);
		XClearWindow(display_, ev.window);
		XDrawString(display_, ev.window, XDefaultGC(display_, DefaultScreen(display_)), 10, START_MENU_HEIGHT / 2 + 4, START_MENU_OPTIONS[drop_index_[ev.window]].c_str(), START_MENU_OPTIONS[drop_index_[ev.window]].length());

	}

}
