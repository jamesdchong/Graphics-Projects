CC = g++
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED -DOSX -Wno-deprecated-register -Wno-deprecated-declarations -Wno-shift-op-parentheses
INCFLAGS = -I./glm-0.9.7.1 -I/usr/X11/include -I./include/
LDFLAGS = -framework GLUT -framework OpenGL -L./lib/mac/ \
		-L"/usr/local/lib" \
		-lGL -lGLU -lm -lstdc++ -lfreeimage

RM = /bin/rm -f 
all: transforms
transforms: main.o shaders.o Transform.o readfile.o display.o variables.h readfile.h shaders.h Transform.h grader.o UCSD/grader.h \
	   Geometry.o
	$(CC) $(CFLAGS) -o transforms shaders.o main.o Transform.o Geometry.o readfile.o display.o grader.o $(INCFLAGS) $(LDFLAGS) 
main.o: main.cpp shaders.h Transform.h variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
shaders.o: shaders.cpp shaders.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
readfile.o: readfile.cpp readfile.h variables.h 
	$(CC) $(CFLAGS) $(INCFLAGS) -c readfile.cpp
display.o: display.cpp variables.h Geometry.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c display.cpp
Transform.o: Transform.cpp Transform.h 
	$(CC) $(CFLAGS) $(INCFLAGS) -c Transform.cpp  
grader.o: UCSD/grader.cpp UCSD/grader.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c UCSD/grader.cpp
Geometry.o: Geometry.cpp Geometry.h Transform.h variables.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Geometry.cpp
clean: 
	$(RM) *.o transforms *.png


 
