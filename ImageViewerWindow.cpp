#include "ImageViewerWindow.hpp"
#include "CustomScrollableWindow.hpp"
#include <gtkmm/image.h>
#include <gtkmm/scrolledwindow.h>
#include <iostream>

ImageViewerWindow::ImageViewerWindow(
    BaseObjectType *cobject,
    const Glib::RefPtr<Gtk::Builder> &refBuilder)
    : Gtk::ApplicationWindow(cobject),
      m_refBuilder(refBuilder),
      m_stack(nullptr),
      m_menu_button(nullptr)
{
    m_stack = m_refBuilder->get_widget<Gtk::Stack>("stack");
    if (!m_stack)
    {
        throw std::runtime_error("ImageViewerWindow::ImageViewerWindow(): No \"stack\" object in window.ui");
    }

    m_menu_button = m_refBuilder->get_widget<Gtk::MenuButton>("menu");
    if (!m_menu_button)
    {
        throw std::runtime_error("ImageViewerWindow::ImageViewerWindow(): No \"menu\" object in window.ui");
    }

    auto menuBuilder = Gtk::Builder::create_from_resource("/org/mt/imageviewer/menuItems.ui");
    auto menu = menuBuilder->get_object<Gio::MenuModel>("menu");
    if (!menu)
    {
        throw std::runtime_error("ImageViewerWindow::ImageViewerWindow(): No \"menu\" object in menuItems.ui");
    }
    m_menu_button->set_menu_model(menu);

    add_action(
        "save",
        sigc::mem_fun(
            *this,
            &ImageViewerWindow::save_image_view));
}

ImageViewerWindow *ImageViewerWindow::create()
{
    auto refBuilder = Gtk::Builder::create_from_resource("/org/mt/imageviewer/window.ui");
    auto window = Gtk::Builder::get_widget_derived<ImageViewerWindow>(refBuilder, "app_window");
    if (!window)
    {
        throw std::runtime_error("ImageViewerWindow::create(): No \"app_window\" in window.ui");
    }
    return window;
}

void ImageViewerWindow::open_image_view(Glib::RefPtr<Gio::File> &file)
{
    auto name = file->get_basename();
    auto drawingArea = Gtk::make_managed<CustomDrawingArea>(file);
    auto scrolledWindow = Gtk::make_managed<CustomScrollableWindow>(drawingArea);
    scrolledWindow->set_expand(true);
    scrolledWindow->set_child(*drawingArea);
    m_stack->add(*scrolledWindow, name, name);
}

void ImageViewerWindow::save_image_view()
{
    auto tab = dynamic_cast<CustomScrollableWindow *>(m_stack->get_visible_child());
    if (!tab)
    {
        throw std::runtime_error("ImageViewerWindow::save_image_view(): No visible tab");
    }
    auto area = tab->get_drawing_area();
    if (!area)
    {
        throw std::runtime_error("ImageViewerWindow::save_image_view(): No drawing area");
    }
    area->set_save_signal();
}