#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/stack.h>
#include <gtkmm/menubutton.h>
#include <gtkmm/colorchooserdialog.h>
#include <gtkmm/colorbutton.h>

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
    void on_color_set();
    /**
     * Creates and opens image view
     */
    void open_image_view(Glib::RefPtr<Gio::File> &file);

private:
    void save_image_view();

protected:
    // Stores a pointer to builder containing window.ui
    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    // Stores a pointer to stack layout ui
    Gtk::Stack *m_stack;
    Gtk::MenuButton *m_menu_button;
    Gtk::ColorButton *m_color_button;
    Gdk::RGBA m_color;
};