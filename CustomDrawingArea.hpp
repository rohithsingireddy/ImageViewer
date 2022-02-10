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
    CustomDrawingArea(Glib::RefPtr<Gio::File> file, Gdk::RGBA color);

    /**
     * Destructor
     */
    virtual ~CustomDrawingArea();

    /**
     * Sets save signal
     */
    bool set_save_signal();

    /**
     * Sets the color used in visible
     * drawing area tab
     */
    void set_color(Gdk::RGBA color);

    /**
     * Gets the color used in visible
     * drawing area tab
     */
    Gdk::RGBA get_color();

    /**
     * Sets the size used in visible
     * drawing area tab
     */
    void set_radius(double radius);

    /**
     * Gets the size used in visible
     * drawing area tab
     */
    double get_radius();

    /**
     * Sets the alpha of color used by
     * visible drawing area tab
     */
    void set_color_alpha(double alpha);

    /**
     * Gets the alpha of color used by
     * visible drawing area
     */
    double get_color_alpha();

protected:
    // A pixel buffer storing the image to render
    Glib::RefPtr<Gdk::Pixbuf> m_image;
    // A reference pointer to the image which is being rendered
    Glib::RefPtr<Gio::File> m_file;

    /**
     * A function which is called everytime the screen is drawn by the application
     */
    void on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height);

    /**
     * A function which is called by on_draw to create circles that serve as user drawn
     * lines
     */ 
    void draw_circle(const Cairo::RefPtr<Cairo::Context> &cr, int x, int y, int radius);

private:
    // For storing every change made by user on drawing area
    struct changes
    {
        double x, y, radius;
        Gdk::RGBA color;
        changes(double a, double b, Gdk::RGBA c, double r);
    };
    //For use with gestureDrag even
    double m_drag_x, m_drag_y, m_offset_x, m_offset_y;
    //For use in drawing user changes
    double m_radius, m_alpha;
    //Used in signalling application to save the visible area
    bool m_signal_to_save;

    //To store user changes
    std::vector<changes> m_changes;
    //Current color used by the user
    Gdk::RGBA m_color;

    /**
     * For use with GestureDrag Event when drag starts
     */
    void on_drag_begin(double start_x, double start_y);

    /**
     * For use with GestureDrag Event when drag updates
     */ 
    void on_drag_update(double offset_x, double offset_y);

    /**
     * For use with GestureDrag Event when drag ends
     */ 
    void on_drag_end(double offset_x, double offset_y);
};

#endif /* CUSTOM_DRAWING_AREA_GUARD */