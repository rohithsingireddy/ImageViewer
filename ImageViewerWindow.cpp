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
      m_menu_button(nullptr),
      m_color_button(nullptr),
      m_color(0.0, 0.0, 1.0),
      m_radius_scale(nullptr),
      m_alpha_scale(nullptr)
{
    m_stack = m_refBuilder->get_widget<Gtk::Stack>("stack");
    if (!m_stack)
    {
        throw std::runtime_error("ImageViewerWindow::ImageViewerWindow(): No \"stack\" object in window.ui");
    }
    m_stack->property_visible_child()
        .signal_changed()
        .connect(
            sigc::mem_fun(
                *this,
                &ImageViewerWindow::on_visible_child_change));

    m_menu_button = m_refBuilder->get_widget<Gtk::MenuButton>("menu");
    if (!m_menu_button)
    {
        throw std::runtime_error("ImageViewerWindow::ImageViewerWindow(): No \"menu\" object in window.ui");
    }

    m_color_button = m_refBuilder->get_widget<Gtk::ColorButton>("color_label");
    if (!m_color_button)
    {
        throw std::runtime_error("ImageViewerWindow::ImageViewerWindow(): No \"color_label\" in window.ui");
    }
    m_color_button->set_rgba(m_color);
    m_color_button->signal_color_set().connect(
        sigc::mem_fun(
            *this,
            &ImageViewerWindow::on_color_set),
        false);

    m_radius_scale = m_refBuilder->get_widget<Gtk::Scale>("radius_slider");
    if (!m_radius_scale)
    {
        throw std::runtime_error("ImageViewerWindow::ImageViewerWindow(): No \"radius_slider\" object in window.ui");
    }
    m_radius_scale->set_value(5.0);
    m_radius_scale->signal_value_changed().connect(
        sigc::mem_fun(
            *this,
            &ImageViewerWindow::on_radius_slider_change),
        false);

    m_alpha_scale = m_refBuilder->get_widget<Gtk::Scale>("alpha_slider");
    if (!m_alpha_scale)
    {
        throw std::runtime_error("ImageViewerWindow::ImageViewerWindow(): No \"alpha_slider\" object in window.ui");
    }
    m_alpha_scale->set_value(1);
    m_alpha_scale->signal_value_changed().connect(
        sigc::mem_fun(
            *this,
            &ImageViewerWindow::on_alpha_slider_change));

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
    auto drawingArea = Gtk::make_managed<CustomDrawingArea>(file, m_color);
    auto scrolledWindow = Gtk::make_managed<CustomScrollableWindow>(drawingArea);
    scrolledWindow->set_expand(true);
    scrolledWindow->set_child(*drawingArea);
    m_stack->add(*scrolledWindow, name, name);
}

void ImageViewerWindow::save_image_view()
{
    auto area = get_current_drawing_area();
    area->set_save_signal();
}

void ImageViewerWindow::on_color_set()
{
    m_color = m_color_button->get_rgba();
    auto area = get_current_drawing_area();
    area->set_color(m_color);
}

void ImageViewerWindow::on_radius_slider_change()
{
    auto area = get_current_drawing_area();
    area->set_radius(m_radius_scale->get_value());
}

void ImageViewerWindow::on_alpha_slider_change()
{
    auto area = get_current_drawing_area();
    area->set_color_alpha(m_alpha_scale->get_value());
}

CustomDrawingArea *ImageViewerWindow::get_current_drawing_area()
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
    return area;
}

void ImageViewerWindow::on_visible_child_change()
{
    auto area = get_current_drawing_area();
    m_alpha_scale->set_value(area->get_color_alpha());
    m_radius_scale->set_value(area->get_radius());
    auto color = area->get_color();
    color.set_alpha(1.0);
    m_color_button->set_rgba(color);
}