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
    if (window.size() > 0)
    {
        app_window = dynamic_cast<ImageViewerWindow *>(window[0]);
    }
    try
    {
        if (!app_window)
        {
            app_window = create_window();
        }
        for (auto file : files)
        {
            app_window->open_image_view(file);
        }
        app_window->present();
    }
    catch (const std::exception &e)
    {
        std::cerr << "ImageViewer::on_open(): " << e.what() << std::endl;
    }
}

void ImageViewer::on_startup()
{
    Gtk::Application::on_startup();

    add_action(
        "quit",
        sigc::mem_fun(
            *this,
            &ImageViewer::on_action_quit_app));
    set_accel_for_action("app.quit", "<Ctrl>Q");

    add_action(
        "open",
        sigc::mem_fun(
            *this,
            &ImageViewer::open_file_dialog));
    set_accel_for_action("app.open", "<Ctrl>O");

    set_accel_for_action("win.save", "<Ctrl>S");
}

void ImageViewer::on_action_quit_app()
{
    auto windows = get_windows();
    for (auto window : windows)
    {
        window->hide();
    }
    quit();
}

void ImageViewer::open_file_dialog()
{
    Gtk::FileChooserDialog *dialog = new Gtk::FileChooserDialog("Choose an image", Gtk::FileChooser::Action::OPEN);

    dialog->set_transient_for(*get_active_window());
    dialog->set_modal(true);

    dialog->add_button("_Cancel", Gtk::ResponseType::CANCEL);
    dialog->add_button("_Open", Gtk::ResponseType::OK);

    auto filter_image = Gtk::FileFilter::create();
    filter_image->set_name("images");
    filter_image->add_mime_type("image/jpeg");
    filter_image->add_mime_type("image/jpg");
    filter_image->add_mime_type("image/png");

    auto filter_all = Gtk::FileFilter::create();
    filter_all->set_name("All");
    filter_all->add_mime_type("*");

    dialog->add_filter(filter_image);
    dialog->add_filter(filter_all);

    dialog->signal_response().connect(
        sigc::bind(
            sigc::mem_fun(
                *this,
                &ImageViewer::on_image_choose),
            dialog));
    
    dialog->show();
}

void ImageViewer::on_image_choose(int response_id, Gtk::FileChooserDialog *dialog)
{
    switch (response_id)
    {
    case Gtk::ResponseType::OK:
    {
        auto file = dialog->get_file();
        ImageViewerWindow *app_window = nullptr;
        auto windows = get_windows();
        if (windows.size() > 0)
        {
            app_window = dynamic_cast<ImageViewerWindow *>(windows[0]);
        }

        try
        {
            if (!app_window)
            {
                app_window = create_window();
            }
            app_window->open_image_view(file);
            app_window->present();
        }
        catch (const Glib::Error &er)
        {
            std::cerr << "Image_Viewer::on_image_choose: " << er.what() << std::endl;
        }
        catch (const std::exception &ex)
        {
            std::cerr << "Image_Viewer::on_image_choose: " << ex.what() << std::endl;
        }
        break;
    }

    default:
    {
        break;
    }
    }
    delete dialog;
}