#include "ImageViewerWindow.hpp"
#include <gtkmm/image.h>
#include <gtkmm/scrolledwindow.h>

ImageViewerWindow::ImageViewerWindow(
    BaseObjectType *cobject,
    const Glib::RefPtr<Gtk::Builder> &refBuilder)
    : Gtk::ApplicationWindow(cobject),
      m_refBuilder(refBuilder),
      m_stack(nullptr)
{
    m_stack = m_refBuilder->get_widget<Gtk::Stack>("stack");
    if(!m_stack)
    {
        throw std::runtime_error("ImageViewerWindow::ImageViewerWindow(): No \"stack\" object in window.ui");
    }
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

void ImageViewerWindow::open_image_view(Glib::RefPtr<Gio::File> &file)
{
    auto imageView = Gtk::make_managed<Gtk::Image>(file->get_path());
    auto name = file->get_basename();
    auto scrolledWindow = Gtk::make_managed<Gtk::ScrolledWindow>();
    scrolledWindow->set_expand(true);
    scrolledWindow->set_child(*imageView);
    m_stack->add(*scrolledWindow, name, name);

}