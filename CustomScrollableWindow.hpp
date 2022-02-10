#include <gtkmm/scrolledwindow.h>
#include "CustomDrawingArea.hpp"

class CustomScrollableWindow : public Gtk::ScrolledWindow
{
private:
    //To store the pointer to drawing area this object presents
    CustomDrawingArea *m_drawing_area;

public:
    /**
     * Constructor
     */ 
    CustomScrollableWindow(CustomDrawingArea *area);

    /**
     * Returns the pointer to the drawing area this object presents
     */ 
    CustomDrawingArea *get_drawing_area();
};