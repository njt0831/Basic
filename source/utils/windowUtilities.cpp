#include "windowUtilities.hpp"

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

void setCircular(Display* display_, Window window){

	XRectangle recs[200];

	XWindowAttributes windowAttributes;
	XGetWindowAttributes(display_, window, &windowAttributes);
       
	if (windowAttributes.width != windowAttributes.height){
	
		return;
		
	}

	int radius = windowAttributes.width / 2;

	int x, y;

	for(int i=0;i<radius / 2;i++){

		y = sin(i * (3.14159 / radius)) * radius;
		x = cos(i * (3.14159 / radius)) * radius;

		recs[i].x = radius;
		recs[i].y = radius;
		recs[i].width = x;
		recs[i].height = y;

	}

	for(int i=(radius / 2);i<radius;i++){

		y = sin((i - (radius / 2)) * (3.14159 / radius)) * radius;
		x = cos((i - (radius / 2)) * (3.14159 / radius)) * radius;

		recs[i].x = radius - x;
		recs[i].y = radius;
		recs[i].width = x;
		recs[i].height = y;

	}

	for(int i=radius;i<radius + (radius / 2);i++){

		y = sin((i - radius) * (3.14159 / radius)) * radius;
		x = cos((i - radius) * (3.14159 / radius)) * radius;

		recs[i].x = radius;
		recs[i].y = radius - y;
		recs[i].width = x;
		recs[i].height = y;

	}

	for(int i=(radius + (radius / 2));i<radius * 2;i++){

		y = sin((i - (radius + (radius / 2))) * (3.14159 / radius)) * radius;
		x = cos((i - (radius + (radius / 2))) * (3.14159 / radius)) * radius;

		recs[i].x = radius - x;
		recs[i].y = radius - y;
		recs[i].width = x;
		recs[i].height = y;

	}

	
	XShapeCombineRectangles(display_, window, ShapeBounding, 0, 0, recs, windowAttributes.width, ShapeSet, 0);

}
