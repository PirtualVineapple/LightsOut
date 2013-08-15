all:
	g++ src/*.cpp -L./ -lglew32 -lglfw3 -lopenGL32 -Iinclude -o LightsOut