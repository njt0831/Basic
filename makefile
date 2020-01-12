source = Basic.cpp BasicMainloop.cpp Main.cpp
flags = -lX11 -lXcursor `pkg-config --cflags --libs opencv4` -std=c++17
executables = basic setwin setvis

all: $(executables)

basic : $(source)
	g++ $(source) -o /bin/Basic $(flags)

setwin : ./misc/setWindow.c
	g++ ./misc/setWindow.c -o /bin/setwin -lX11

setvis : ./misc/visibility.c
	g++ ./misc/visibility.c -o /bin/setvis -lX11
