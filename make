#!/bin/bash
g++ -std=c++17 Basic.cpp BasicMainloop.cpp Main.cpp -o /bin/Basic -lX11 -lXcursor `pkg-config --cflags --libs opencv4`
g++ /root/Basic/misc/setWindow.c -o /bin/setwin -lX11
g++ /root/Basic/misc/visibility.c -o /bin/visibility -lX11
