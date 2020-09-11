#include <X11/Xlib.h>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>

void loadResource(Display* display_, Window root_, const char* resourcePath, Window window, unsigned int width, unsigned int height){

	cv::Mat img = cv::imread(resourcePath);
	XImage* xImage = XCreateImage(display_, XDefaultVisual(display_, DefaultScreen(display_)), 24, ZPixmap, 0, (char*) img.data, width * .75, height, 32, width*3);
	xImage->bits_per_pixel = 24;
        xImage->width = width;
	Pixmap pixmap = XCreatePixmap(display_, root_, width, height, 24);
	XPutImage(display_, pixmap, XDefaultGC(display_, DefaultScreen(display_)), xImage, 0, 0, 0, 0, width, height);
	XSetWindowBackgroundPixmap(display_, window, pixmap);
	XClearWindow(display_, window);
	XFreePixmap(display_, pixmap);

}
