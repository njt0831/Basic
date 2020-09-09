source = ./source/Basic.cpp ./source/handlers/buttonPress.cpp ./source/handlers/buttonRelease.cpp ./source/handlers/configureRequest.cpp ./source/handlers/enterNotify.cpp ./source/handlers/leaveNotify.cpp ./source/handlers/mapRequest.cpp ./source/handlers/motionNotify.cpp ./source/handlers/unmapNotify.cpp ./source/Main.cpp
flags = -lX11 -lXcursor `pkg-config --cflags --libs opencv4` -std=c++17
executables = basic

all: $(executables)

basic : $(source)
	g++ $(source) -o /bin/Basic $(flags)
