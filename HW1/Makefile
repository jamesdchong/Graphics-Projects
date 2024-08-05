CC = g++
CFLAGS = -g -DGL_GLEXT_PROTOTYPES -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED -DOSX -Wno-deprecated-register -Wno-deprecated-declarations -Wno-shift-op-parentheses -Wno-parentheses-equality
INCFLAGS = -I./glm-0.9.7.1 -I/usr/X11/include -I./include/
LDFLAGS = -framework GLUT -framework OpenGL -L./lib/osx/ \
		-L"/usr/local/lib" \
		-lGL -lGLU -lm -lstdc++ -lfreeimage

RM = /bin/rm -f 
all: transforms

transforms: main.o shaders.o Transform.o Geometry.o shaders.h Transform.h Geometry.h grader.o UCSD/grader.h
	$(CC) $(CFLAGS) -o transforms main.o shaders.o Transform.o Geometry.o grader.o $(INCFLAGS) $(LDFLAGS) 
main.o: main.cpp shaders.h Geometry.h Transform.h UCSD/grader.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
shaders.o: shaders.cpp shaders.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
Transform.o: Transform.cpp Transform.h 
	$(CC) $(CFLAGS) $(INCFLAGS) -c Transform.cpp  
grader.o: UCSD/grader.cpp UCSD/grader.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c UCSD/grader.cpp
Geometry.o: Geometry.cpp Geometry.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Geometry.cpp
clean: 
	$(RM) *.o transforms *.png
