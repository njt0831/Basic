#include <X11/Xlib.h>
#include <X11/extensions/shape.h>
#include <opencv4/opencv2/core/core.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>

void loadResource(Display* display_, Window root_, const char* resourcePath, Window window, unsigned int width, unsigned int height);
void setCircular(Display* display_, Window window); 
