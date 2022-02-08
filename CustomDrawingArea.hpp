#ifndef CUSTOM_DRAWING_AREA_GUARD
#define CUSTOM_DRAWING_AREA_GUARD

#include <gdkmm/pixbuf.h>
#include <gtkmm/drawingarea.h>

class CustomDrawingArea: public Gtk::DrawingArea
{
public:
    /**
     * Constructor
     */ 
    CustomDrawingArea(std::string fileName);

    /**
     * Destructor
     */ 
    virtual ~CustomDrawingArea();

protected:
    /**
     */ 
    void on_draw(const Cairo::RefPtr<Cairo::Context>& cr, int width, int height);

    Glib::RefPtr<Gdk::Pixbuf> m_image;
};

#endif /* CUSTOM_DRAWING_AREA_GUARD */