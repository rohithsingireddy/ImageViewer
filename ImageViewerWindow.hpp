#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/stack.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/colorbutton.h>
#include <gtkmm/scale.h>

#include "CustomDrawingArea.hpp"

class ImageViewerWindow : public Gtk::ApplicationWindow
{
public:
    /**
     * Constructor
     */
    ImageViewerWindow(
        BaseObjectType *cobject,
        const Glib::RefPtr<Gtk::Builder> &refBuilder);
    /**
     * Creats an ImageViewerWindow object and
     * returns a pointer to it
     */
    static ImageViewerWindow *create();
    /**
     * Sets the color of the drawing area in view
     */ 
    void on_color_set();
    /**
     * Creates and opens image view
     */
    void open_image_view(Glib::RefPtr<Gio::File> &file);

private:
    /**
     * Gets the pointer to the current drawing area
     */ 
    CustomDrawingArea *get_current_drawing_area();

    /**
     * Saves the visible drawing area in the file that 
     * is opened
     */ 
    void save_image_view();

    /**
     * Called whenever the size slider is being changed
     */ 
    void on_radius_slider_change();

    /**
     * Called whenever the alpha slider is being changed
     */ 
    void on_alpha_slider_change();

    /**
     * Called whenever the stack visible child is changed
     */ 
    void on_visible_child_change();

protected:
    // Stores a pointer to builder containing window.ui
    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    // Stores a pointer to stack in ui
    Gtk::Stack *m_stack;
    // Stores a pointer to menu button in ui
    Gtk::MenuButton *m_menu_button;
    // Stores a pointer to color picker button in ui
    Gtk::ColorButton *m_color_button;
    // Stores pointers to sliders in ui
    Gtk::Scale *m_radius_scale, *m_alpha_scale;
    //Stores the selected color being used by user in visible drawing area
    Gdk::RGBA m_color;
};