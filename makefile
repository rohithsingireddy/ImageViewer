PROGRAM = ImageViewer
CFLAGS = $(shell pkg-config --cflags --libs gtkmm-4.0)
DEPS = ImageViewer.hpp ImageViewerWindow.hpp CustomDrawingArea.hpp CustomScrollableWindow.hpp
CXXFILES = CustomDrawingArea.cpp ImageViewerWindow.cpp ImageViewer.cpp main.cpp CustomScrollableWindow.cpp
OFILES = CustomDrawingArea.o ImageViewerWindow.o ImageViewer.o main.o ui_resources/resources.o CustomScrollableWindow.o
UI_FILES = ui_resources/menuItems.ui ui_resources/window.ui

all: ImageViewer

ui_resources/imageViewer.gresource.xml: $(UI_FILES)

ImageViewer: $(OFILES) ui_resources/resources.o 
	g++ -o $@ $^ $(CFLAGS)

%.o : %.cpp $(DEPS)
	g++ -c -o $@ $< $(CFLAGS)

ui_resources/resources.o : ui_resources/resources.c
	g++ -c -o $@ $^ $(CFLAGS)

ui_resources/resources.c: ui_resources/imageViewer.gresource.xml $(UI_FILES)
	glib-compile-resources --target=ui_resources/resources.c --generate-source --sourcedir=ui_resources/ $<


clean:
	rm $(OFILES) -f
	rm ui_resources/resources.c
	# rm ./ImageViewer