handlers = buttonPress.o buttonRelease.o configureRequest.o leaveNotify.o motionNotify.o enterNotify.o mapRequest.o unmapNotify.o
utils = windowUtilities.o
main = Basic.o Main.o

objects = $(handlers) $(utils) $(main)

flags = -lX11 -lXext -lXcursor -lXrandr `pkg-config --cflags --libs opencv4` -std=c++17 -I./source/headers

all: basic

$(handlers) : %.o: ./source/handlers/%.cpp
	g++ -c $< -o ./build/$@ $(flags)

$(utils) : %.o: ./source/utils/%.cpp
	g++ -c $< -o ./build/$@ $(flags)

$(main) : %.o: ./source/%.cpp
	g++ -c $< -o ./build/$@ $(flags)


basic : $(objects)
	g++ ./build/* -o ./build/Basic $(flags)

install:
	@cp -f ./build/Basic /bin/Basic

clean:
	@rm -f ./build/*
