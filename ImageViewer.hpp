#ifndef IMAGE_VIEWER_GUARD
#define IMAGE_VIEWER_GUARD

#include <gtkmm/application.h>
#include <gtkmm/filechooserdialog.h>
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

    /**
     * Method to call when the app is quit
     */
    void on_action_quit_app(); 

    /**
     * Opens a file dialog for choosing images
     */
    void open_file_dialog(); 

    /**
     * Opens a file choosen by file dialog
     */
    void on_image_choose(int response_id, Gtk::FileChooserDialog *dialog); 

protected:
    /**
     * Overriding a Gtk::Application method
     */
    void on_activate() override;

    /**
     * Overriding a Gtk::Application method
     */
    void on_open(
        const Gio::Application::type_vec_files &files,
        const Glib::ustring &);

    /**
     * Overriding a Gtk::Application method
     */
    void on_startup() override;



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