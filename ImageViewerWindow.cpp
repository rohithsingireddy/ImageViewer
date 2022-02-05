#include "ImageViewerWindow.hpp"

ImageViewerWindow::ImageViewerWindow(
    BaseObjectType *cobject,
    const Glib::RefPtr<Gtk::Builder> &refBuilder)
    : Gtk::ApplicationWindow(cobject),
      m_refBuilder(refBuilder),
      m_stack(nullptr)
{
    
}

ImageViewerWindow *ImageViewerWindow::create()
{
    auto refBuilder = Gtk::Builder::create_from_resource("/org/mt/imageviewer/window.ui");
    auto window = Gtk::Builder::get_widget_derived<ImageViewerWindow>(refBuilder, "app_window");
    if(!window)
    {
        throw std::runtime_error("ImageViewerWindow::create(): No \"app_window\" in window.ui");
    }
    return window;
}