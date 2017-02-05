CFLAGS=-c -g -O0 -fmerge-constants
CFLAGS+=-Wall $(shell pkg-config --cflags libdrm)

CXXFLAGS=-std=c++11 $(CFLAGS)

GLES-DEMO-OBJECTS = main.o gles-renderer.o util.o egl-context.o x11-egl.o
GTK-DEMO-OBJECTS = gtk-gl-main.o gles-renderer.o util.o

LDLIBS = -lEGL -lGLESv2 -lgbm -ldrm -lX11
GTKLIBS = $(shell pkg-config --libs gtk+-3.0) -lGLESv2

all : gles-demo gtk-demo

gles-demo : $(GLES-DEMO-OBJECTS)
	g++ -o $@ $(GLES-DEMO-OBJECTS) $(LDLIBS)

$(GTK-DEMO-OBJECTS) : CFLAGS += $(shell pkg-config --cflags gtk+-3.0)
gtk-demo : $(GTK-DEMO-OBJECTS)
	g++ -o $@ $(GTK-DEMO-OBJECTS) $(GTKLIBS)

clean :
	rm -f *.o
	rm -f gles-demo gtk-demo

%.o: %.c dummy
	$(CC) $(CFLAGS) -c $<

%.o: %.cpp dummy
	$(CXX) $(CXXFLAGS) $(CFLAGS) -c $<
