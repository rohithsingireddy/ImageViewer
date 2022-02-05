#include <gtkmm/applicationwindow.h>
#include <gtkmm/builder.h>
#include <gtkmm/stack.h>

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

protected:
    
    // Stores a pointer to builder containing window.ui 
    Glib::RefPtr<Gtk::Builder> m_refBuilder; 
    // Stores a pointer to stack layout ui
    Glib::RefPtr<Gtk::Stack> m_stack;
};