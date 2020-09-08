#include "Basic.hpp"

void Basic::handleUnmapNotify(XUnmapNotifyEvent ev){

	if (!(client-frame_.count(ev.xunmap.window)) || (ev.xunmap.event == root_) || frame-client_.count(ev.xunmap.window) || close-client_.count(ev.xunmap.window)){return;}

	tempWindow = client-frame_[ev.xunmap.window];

	if (mouseHook && hookWin == tempWindow){
		
		mouseHook = 0;
				
	}

	XUnmapWindow(display_, tempWindow);
	XDestroyWindow(display_, frame-close_[tempWindow]);
	XDestroyWindow(display_, tempWindow);

	client-frame_.erase(ev.xunmap.window);
	frame-client_.erase(tempWindow);
	close-client_.erase(frame-close_[tempWindow]);
	minimize-client_.erase(client-minimize_[ev.xunmap.window]);
	client-minimize_.erase(ev.xunmap.window);
	frame-close_.erase(tempWindow);
	
}
