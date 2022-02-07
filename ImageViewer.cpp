#include "ImageViewer.hpp"
#include <iostream>

ImageViewer::ImageViewer()
    : Gtk::Application(
          "org.mt.imageviewer.application",
          Gio::Application::Flags::HANDLES_OPEN)
{
}

Glib::RefPtr<ImageViewer> ImageViewer::create()
{
    return Glib::make_refptr_for_instance<ImageViewer>(new ImageViewer());
}

ImageViewerWindow *ImageViewer::create_window()
{
    auto window = ImageViewerWindow::create();
    add_window(*window);
    window->signal_hide().connect(
        sigc::bind(
            sigc::mem_fun(
                *this,
                &ImageViewer::on_hide_window),
            window));
    return window;
}

void ImageViewer::on_hide_window(Gtk::Window *window)
{
    delete window;
}

void ImageViewer::on_activate()
{
    try
    {
        create_window()->present();
    }
    catch (const Glib::Error &ex)
    {
        std::cerr << "ImageViewer::on_activate(): " << ex.what() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "ImageViewer::on_activate(): " << e.what() << std::endl;
    }
}

void ImageViewer::on_open(
    const Gio::Application::type_vec_files &files,
    const Glib::ustring &)
{
    ImageViewerWindow *app_window = nullptr;
    auto window = get_windows();
    if( window.size() > 0 )
    {
        app_window = dynamic_cast<ImageViewerWindow *>(window[0]);
    }
    try
    {
        if(!app_window)
        {
            app_window = create_window();
        }
        for( auto file: files )
        {
            app_window->open_image_view(file);
        }
        app_window->present();
    }
    catch(const std::exception& e)
    {
        std::cerr << "ImageViewer::on_open(): " << e.what() << std::endl;
    }
    
}

void ImageViewer::on_startup()
{
    Gtk::Application::on_startup();

    add_action(
        "quit"
    );
}