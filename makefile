source		= main.c functions.c
objects		= main.o functions.o
option		= -Wall
compile 	= -Wall -c
build		= -Wall -o
library		= -lm -lglfw -lGL -lGLU -lGLEW 
target		= main
executable	= ./main
memflags	= --tool=memcheck --leak-check=yes
memtest		= valgrind

compile:
	$(param) $(compile) $(source) $(library)

build:
	$(param) $(build) $(target) $(objects) $(library)

clean: 
	rm $(target) *.o 

memtest:
	$(memtest) $(memflags) $(executable)	
