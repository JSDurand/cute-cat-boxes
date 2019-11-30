CC = g++
CCFLAGS = -Wall -g
INCLUDE = -I /Users/durand/Desktop/Centre/A\ propos\ de\ programmes/CPP/learn\ opengl\ tutorial/include
LIBS = -L /Users/durand/Desktop/Centre/A\ propos\ de\ programmes/CPP/learn\ opengl\ tutorial/lib-macos -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

main: main.cpp
	$(CC) $(CCFLAGS) $(INCLUDE) -x c++ main.cpp stb_image.cpp shader.cpp -x c glad.c -o main $(LIBS)
