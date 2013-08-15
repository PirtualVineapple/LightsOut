all:
	g++ src/*.cpp -L./ -lglew32 -lglfw3 -Iinclude -o LightsOut