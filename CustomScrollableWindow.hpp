#include <gtkmm/scrolledwindow.h>
#include "CustomDrawingArea.hpp"

class CustomScrollableWindow : public Gtk::ScrolledWindow
{
private:
    CustomDrawingArea *m_drawing_area;

public:
    CustomScrollableWindow(CustomDrawingArea *area);
    CustomDrawingArea *get_drawing_area();
};