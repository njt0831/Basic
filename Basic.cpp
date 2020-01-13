#include "Basic.hpp"

Basic Basic::Create(){

	Display* display = XOpenDisplay(nullptr);

	if (display == nullptr){

		return nullptr;
	
	}

	return *(new Basic(display));
}

Basic::Basic(Display* display){

	display_ = display;
	root_ = DefaultRootWindow(display_);
	XSelectInput(display_, root_, SubstructureRedirectMask | SubstructureNotifyMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask);
	
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	srand((unsigned int) currentTime.tv_sec + currentTime.tv_usec);

	// Probably a not stupid way to do this but fuck it its like 6 files
	backgroundCount = -1;
	backgroundPath = "/root/Basic/resources/backgrounds";
	
	for(auto& file: std::filesystem::directory_iterator(backgroundPath)){
		backgroundCount++;
	}

	currentBackground = -1;
	NextRandomDesktopBackground();
	cursor = XCreateFontCursor(display_, 132);
	XDefineCursor(display_, root_, cursor);
	dropdown = 0;
	dropshowflag = 0;
	
	f = fopen("/root/log.txt", "w");

}

void Basic::NextRandomDesktopBackground(){
	
	//Seriously this is fucking retarded do this right when youre not sleepy
	
	int pick = currentBackground;
	
	while (pick == currentBackground){

	       pick = (rand() % backgroundCount);
	
	}
	
	int loopy = 0;

	for(auto& file: std::filesystem::directory_iterator(backgroundPath)){
		
		if ((loopy == pick) && (loopy != currentBackground)){
			
			LoadBackgroundResource(file.path().string().c_str(), root_, 1920, 1080);
			break;

		}

		loopy++;
		
	}

	currentBackground = pick;

}

void Basic::LoadResource(const char* resourcePath, Window window, unsigned int width, unsigned int height){

	img = cv::imread(resourcePath);
	xImage = XCreateImage(display_, XDefaultVisual(display_, DefaultScreen(display_)), 24, ZPixmap, 0, (char*) img.data, width * .75, height, 32, width*3);
	xImage->bits_per_pixel = 24;
        xImage->width = width;
	Pixmap pixmap = XCreatePixmap(display_, root_, width, height, 24);
	XPutImage(display_, pixmap, XDefaultGC(display_, DefaultScreen(display_)), xImage, 0, 0, 0, 0, width, height);
	XSetWindowBackgroundPixmap(display_, window, pixmap);
	XClearWindow(display_, window);
	XFreePixmap(display_, pixmap);

}

void Basic::LoadBackgroundResource(const char* resourcePath, Window window, unsigned int width, unsigned int height){
	
	if (currentBackground < 0){

		img = cv::imread(resourcePath);
		xImageBackground = XCreateImage(display_, XDefaultVisual(display_, DefaultScreen(display_)), 24, ZPixmap, 0, (char*) img.data, width * .75, height, 32, width*3);
		xImageBackground->bits_per_pixel = 24;
       		xImageBackground->width = width;
		backgroundMap = XCreatePixmap(display_, root_, width, height, 24);
		XPutImage(display_, backgroundMap, XDefaultGC(display_, DefaultScreen(display_)), xImageBackground, 0, 0, 0, 0, width, height);
		XSetWindowBackgroundPixmap(display_, window, backgroundMap);
		XClearWindow(display_, window);

	}else{
	
		img = cv::imread(resourcePath);
		xImageBackground = XCreateImage(display_, XDefaultVisual(display_, DefaultScreen(display_)), 24, ZPixmap, 0, (char*) img.data, width * .75, height, 32, width*3);
		xImageBackground->bits_per_pixel = 24;
        	xImageBackground->width = width;
		intermediate = XCreatePixmap(display_, root_, width, height, 24);
		XPutImage(display_, intermediate, XDefaultGC(display_, DefaultScreen(display_)), xImageBackground, 0, 0, 0, 0, width, height);
		XSetWindowBackgroundPixmap(display_, window, intermediate);
		
		curMap = XCreatePixmap(display_, root_, 1920, 1080, 24);
		winMap_[root_] = curMap;
		
		double opacity = 0;
		while (opacity < 1){

			DrawOpaqueWindow(root_, opacity);
			opacity = opacity + .03;

		}

		DrawOpaqueWindow(root_, 1);
		
		xImageBackground = XCreateImage(display_, XDefaultVisual(display_, DefaultScreen(display_)), 24, ZPixmap, 0, (char*) img.data, width * .75, height, 32, width*3);
		xImageBackground->bits_per_pixel = 24;
       		xImageBackground->width = width;
		backgroundMap = XCreatePixmap(display_, root_, width, height, 24);
		XPutImage(display_, backgroundMap, XDefaultGC(display_, DefaultScreen(display_)), xImageBackground, 0, 0, 0, 0, width, height);
		XSetWindowBackgroundPixmap(display_, window, backgroundMap);
		XClearWindow(display_, window);
		XFreePixmap(display_, intermediate);
		
	}


}


void Basic::Initialize(){
	
	ResetGrab();
	XSync(display_, false);	
	XGrabServer(display_);

	Window rootBack, parentBack;
	Window* unframedWindows;
	unsigned int unframedWindowCount;
	XQueryTree(display_, root_, &rootBack, &parentBack, &unframedWindows, &unframedWindowCount);
	//XClassHint test;
	winClass.res_name = NULL;

	for (unsigned int i = 0; i < unframedWindowCount; i++){

		
		XGetWindowAttributes(display_, unframedWindows[i], &winAtt);

		newFrame = XCreateSimpleWindow(display_, root_, winAtt.x, winAtt.y, winAtt.width, winAtt.height + FRAME_TITLE_BAR_WIDTH, FRAME_BORDER_WIDTH, FRAME_BORDER_COLOR, FRAME_COLOR);
		closeButton = XCreateSimpleWindow(display_, root_, 0, 0, 20, 12, 1, 0x181616, 0x363333);
				
		XSelectInput(display_, newFrame, SubstructureNotifyMask | ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);
		XSelectInput(display_, closeButton, ButtonPressMask);	

		XReparentWindow(display_, unframedWindows[i], newFrame, 0, FRAME_TITLE_BAR_WIDTH);
		XReparentWindow(display_, closeButton, newFrame, winAtt.width - 25, 4);

		XMapWindow(display_, newFrame);
		XMapWindow(display_, closeButton);
		LoadResource("/root/Basic/resources/utilities/close.png", closeButton, 20, 12);
	
		clientFrame_[unframedWindows[i]] = newFrame;
		frameClient_[newFrame] = unframedWindows[i];
		killClient_[closeButton] = unframedWindows[i];
		frameKill_[newFrame] = closeButton;
		
		XSetInputFocus(display_, newFrame, RevertToParent, CurrentTime);
		XGrabButton(display_, AnyButton, AnyModifier, newFrame, false, ButtonPressMask | ButtonReleaseMask, GrabModeSync, GrabModeAsync, None, None);

		//fprintf(f, "Mapped Frame: %lld\n", frame);
		//fprintf(f, "Mapped Client: %lld\n", unframedWindows[i]);

	        //XGetWMName(display_, unframedWindows[i], &textProp);
		//fprintf(f, reinterpret_cast<const char*> (textProp.value));
		//XGetClassHint(display_, unframedWindows[i], &test);
		
	}

	XFree(unframedWindows);
	XUngrabServer(display_);

}

void Basic::DrawOpaqueWindow(Window drawFrame, double alphafore){

	XGetWindowAttributes(display_, drawFrame, &winAtt);
	xImage = XGetImage(display_, intermediate, 0, 0, winAtt.width, winAtt.height, ULONG_MAX, ZPixmap);
	xImageBackground = XGetImage(display_, backgroundMap, winAtt.x, winAtt.y, winAtt.width, winAtt.height, ULONG_MAX, ZPixmap); 
	
	frontData = xImage->data;
	backData = xImageBackground->data;
	
	double tempforemult;
	double tempbackmult;
	double tempbotmult;
	double alphaback = 1-alphafore;

	//Probably should do bulk matrix math using Mat structure
	//Possibly multithread per channel

	for (unsigned int i = 0; i < (winAtt.width * winAtt.height * 4);i++){
		
		tempforemult = ((*(frontData + i)) * alphafore);
		tempbackmult = (((*(backData + i)) * alphaback) * (1-alphafore));
		tempbotmult = alphafore + ((alphaback * (1 - alphafore)));

		*(frontData + i) = (char) ((tempforemult + tempbackmult) / tempbotmult);

	
	}

	XPutImage(display_, winMap_[drawFrame], XDefaultGC(display_, DefaultScreen(display_)), xImage, 0, 0, 0, 0, winAtt.width, winAtt.height);
	XSetWindowBackgroundPixmap(display_, drawFrame, winMap_[drawFrame]);
	XClearWindow(display_, drawFrame);
	free(frontData);
	free(backData);


}

void Basic::CloseDisplay(){

	fclose(f);
	XCloseDisplay(display_);

}

void Basic::ResetGrab(){
	
	mouseHook = false;
	resizeLeft = false;
	resizeRight = false;
	resizeDown = false;
	resizeUp = false;
	
}

void Basic::SetPointer(int mx, int my){

	mousex = mx;
	mousey = my;

}

void Basic::SetGrabState(XWindowAttributes winAtt, XButtonPressedEvent e){

	if (e.x < FRAME_BORDER_WIDTH){resizeLeft = true;}
	if (e.x > winAtt.width - FRAME_BORDER_WIDTH){resizeRight = true;}
	if (e.y > winAtt.height - FRAME_BORDER_WIDTH){resizeDown = true;}
	if (e.y < FRAME_BORDER_WIDTH){resizeUp = true;}
	
	if (resizeDown || resizeRight || resizeLeft || resizeUp || (e.y < FRAME_TITLE_BAR_WIDTH)){

		mouseHook = true;
		winHookx = winAtt.x;
		winHooky = winAtt.y;
		winHookWidth = winAtt.width;
		winHookHeight = winAtt.height;
		pHookx = e.x_root;
		pHooky = e.y_root;
		hookWin = e.window;

	}

}

void Basic::Action(){
	
	if (mouseHook){

		if (resizeRight && resizeDown){

			XResizeWindow(display_, hookWin, winHookWidth + (mousex - pHookx), winHookHeight + (mousey - pHooky));
			XResizeWindow(display_, frameClient_[hookWin], (winHookWidth) + (mousex - pHookx), winHookHeight - FRAME_TITLE_BAR_WIDTH + (mousey - pHooky));
			XMoveWindow(display_, frameKill_[hookWin], winHookWidth + (mousex - pHookx) - FRAME_TITLE_BAR_WIDTH - 3, 4);

		}else if (resizeRight){
				
			XResizeWindow(display_, hookWin, winHookWidth + (mousex - pHookx), winHookHeight);
			XResizeWindow(display_, frameClient_[hookWin], (winHookWidth) + (mousex - pHookx), winHookHeight - 22);
			XMoveWindow(display_, frameKill_[hookWin], winHookWidth + (mousex - pHookx) - FRAME_TITLE_BAR_WIDTH - 3, 4);	

		}else if (resizeDown){
			
			XResizeWindow(display_, hookWin, winHookWidth, winHookHeight + (mousey - pHooky));
			XResizeWindow(display_, frameClient_[hookWin], winHookWidth, winHookHeight - FRAME_TITLE_BAR_WIDTH + (mousey - pHooky));

		}else if (resizeLeft){
			//TODO
		}else if (resizeRight){
			//TODO
				
		}else{
			XMoveWindow(display_, hookWin, winHookx + (mousex - pHookx), winHooky + (mousey - pHooky));

		}
	}
}
