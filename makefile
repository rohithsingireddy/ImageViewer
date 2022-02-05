PROGRAM = ImageViewer
CFLAGS = $(shell pkg-config --cflags --libs gtkmm-4.0)
DEPS = ImageViewer.hpp
CXXFILES = ImageViewerWindow.cpp ImageViewer.cpp main.cpp
OFILES = ImageViewerWindow.o ImageViewer.o main.o ui_resources/resources.o

all: ImageViewer

ImageViewer: $(OFILES) ui_resources/resources.o 
	g++ -o $@ $^ $(CFLAGS)

%.o : %.cpp $(DEPS)
	g++ -c -o $@ $< $(CFLAGS)

ui_resources/resources.o : ui_resources/resources.c
	g++ -c -o $@ $^ $(CFLAGS)

ui_resources/resources.c: ui_resources/imageViewer.gresource.xml
	glib-compile-resources --target=ui_resources/resources.c --generate-source --sourcedir=ui_resources/ $^

clean:
	rm $(OFILES) -f
	rm ui_resources/resources.c
	rm ./ImageViewer