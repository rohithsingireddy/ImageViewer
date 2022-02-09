#ifndef CUSTOM_DRAWING_AREA_GUARD
#define CUSTOM_DRAWING_AREA_GUARD

#include <gdkmm/pixbuf.h>
#include <gtkmm/drawingarea.h>
#include <giomm/file.h>

class CustomDrawingArea : public Gtk::DrawingArea
{
public:
    /**
     * Constructor
     */
    CustomDrawingArea(Glib::RefPtr<Gio::File> file);

    /**
     * Destructor
     */
    virtual ~CustomDrawingArea();

    /**
     * Sets save signal
     */
    bool set_save_signal(); 

protected:
    /**
     */
    Glib::RefPtr<Gdk::Pixbuf> m_image;
    Glib::RefPtr<Gio::File> m_file;

    void on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height);
    void draw_circle(const Cairo::RefPtr<Cairo::Context> &cr, int x, int y);

private:
    double m_drag_x, m_drag_y, m_offset_x, m_offset_y;
    bool m_signal_to_save;
    std::vector<std::pair<double, double>> m_changes;
    enum Drag_Status{
        NONE,
        BEGIN,
        UPDATE,
        END
    } m_drag_status;

    void on_drag_begin(double start_x, double start_y);
    void on_drag_update(double offset_x, double offset_y);
    void on_drag_end(double offset_x, double offset_y);
};

#endif /* CUSTOM_DRAWING_AREA_GUARD */