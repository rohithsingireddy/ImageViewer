#ifndef IMAGE_VIEWER_GUARD
#define IMAGE_VIEWER_GUARD

#include <gtkmm/application.h>
#include "ImageViewerWindow.hpp"

class ImageViewer : public Gtk::Application
{
private:
    /**
     * Creates an ImageViewerWindow and returns a
     * pointer to it
     */
    ImageViewerWindow *create_window();

    /**
     * Method to call when the window is hidden
     * from the user
     */
    void on_hide_window(Gtk::Window *window);

protected:
    /**
     * Overrideing a Gtk::Application method
     */
    void on_activate() override;

public:
    /**
     * Constructor
     */
    ImageViewer();

    /**
     * Creates an ImageViewer object and
     * returns a pointer to it
     */
    static Glib::RefPtr<ImageViewer> create();
};

#endif /* IMAGE_VIEWER_GUARD */