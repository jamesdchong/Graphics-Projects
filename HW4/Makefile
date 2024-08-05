CC = clang++
CFLAGS = -O3 -Xpreprocessor -fopenmp -DGL_GLEXT_PROTOTYPES -DGL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED -DOSX -Wno-deprecated-register -Wno-deprecated-declarations -Wno-shift-op-parentheses
INCFLAGS = -I./glm-0.9.7.1 -I/usr/X11/include -I./include/
LDFLAGS = -framework GLUT -L./lib/mac/ \
		-L"/usr/local/lib" -lfreeimage -lomp
		
RM = /bin/rm -f

all: raytracer
raytracer: main.o objects.o readfile.o Transform.o variables.h structs.h objects.h readfile.h Transform.h
	$(CC) $(CFLAGS) -o raytracer main.o objects.o readfile.o Transform.o $(INCFLAGS) $(LDFLAGS)
main.o: variables.h objects.h main.cpp readfile.cpp readfile.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
objects.o: variables.h objects.h objects.cpp
	$(CC) $(CFLAGS) $(INCFLAGS) -c objects.cpp
readfile.o: variables.h readfile.cpp readfile.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c readfile.cpp
Transform.o: variables.h Transform.cpp Transform.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Transform.cpp
clean: 
	$(RM) *.o raytracer *.png output
