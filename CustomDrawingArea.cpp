#include "CustomDrawingArea.hpp"
#include <cairomm/context.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <glibmm/fileutils.h>
#include <gtkmm/gesturedrag.h>
#include <iostream>

CustomDrawingArea::CustomDrawingArea(Glib::RefPtr<Gio::File> file, Gdk::RGBA color)
    : m_file(nullptr),
      m_drag_x(0),
      m_drag_y(0),
      m_offset_x(0),
      m_offset_y(0),
      m_signal_to_save(false),
      m_color(color),
      m_radius(5.0),
      m_alpha(1.0)
{
    m_file = file;
    std::string fileName = m_file->get_basename();
    m_image = Gdk::Pixbuf::create_from_file(fileName);
    if (!m_image)
    {
        throw std::runtime_error("CustomDrawingArea::CustomDrawingArea(): Cannot create from " + fileName);
    }

    set_content_width(m_image->get_width() / 2);
    set_content_height(m_image->get_height() / 2);
    set_draw_func(sigc::mem_fun(
        *this,
        &CustomDrawingArea::on_draw));

    auto m_gesture_drag = Gtk::GestureDrag::create();
    m_gesture_drag->signal_drag_begin().connect(
        sigc::mem_fun(
            *this,
            &CustomDrawingArea::on_drag_begin));

    m_gesture_drag->signal_drag_update().connect(
        sigc::mem_fun(
            *this,
            &CustomDrawingArea::on_drag_update));

    m_gesture_drag->signal_drag_end().connect(
        sigc::mem_fun(
            *this,
            &CustomDrawingArea::on_drag_end));

    add_controller(m_gesture_drag);
}

CustomDrawingArea::~CustomDrawingArea()
{
}

CustomDrawingArea::changes::changes(double a, double b, Gdk::RGBA c, double r = 5)
{
    x = a;
    y = b;
    color = c;
    radius = r;
}

void CustomDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height)
{
    Gdk::Cairo::set_source_pixbuf(
        cr,
        m_image,
        (width - m_image->get_width()) / 2,
        (height - m_image->get_height()) / 2);
    cr->paint();
    cr->save();
    for (auto i : m_changes)
    {
        Gdk::Cairo::set_source_rgba(cr, i.color);
        draw_circle(cr, i.x, i.y, i.radius);
    }
    if (m_signal_to_save)
    {
        cr->get_target()->write_to_png(m_file->get_path());
        m_signal_to_save = false;
    }
    cr->restore();
}

void CustomDrawingArea::draw_circle(const Cairo::RefPtr<Cairo::Context> &cr, int x, int y, int radius = 5)
{
    cr->arc(x, y, radius, 0.0, 2.0 * M_PI); // full circle
    cr->fill_preserve();
    cr->stroke();
}

void CustomDrawingArea::on_drag_begin(double start_x, double start_y)
{
    m_drag_x = start_x;
    m_drag_y = start_y;
    m_changes.push_back(
        CustomDrawingArea::changes(
            start_x, 
            start_y, 
            m_color,
            m_radius));
    queue_draw();
}

void CustomDrawingArea::on_drag_update(double offset_x, double offset_y)
{
    m_changes.push_back(
        CustomDrawingArea::changes(
            m_drag_x + offset_x, 
            m_drag_y + offset_y, 
            m_color,
            m_radius));
    queue_draw();
}

void CustomDrawingArea::on_drag_end(double offset_x, double offset_y)
{
    m_changes.push_back(
        CustomDrawingArea::changes(
            m_drag_x + offset_x, 
            m_drag_y + offset_y, 
            m_color,
            m_radius)); 
    queue_draw();
    m_drag_x = 0;
    m_drag_y = 0;
}

bool CustomDrawingArea::set_save_signal()
{
    m_signal_to_save = true;
    queue_draw();
    return true;
}

void CustomDrawingArea::set_color(Gdk::RGBA color)
{
    m_color = color;
    m_color.set_alpha(m_alpha);
}

void CustomDrawingArea::set_radius(double radius)
{
    m_radius = radius;
}

void CustomDrawingArea::set_color_alpha(double alpha = 1.0)
{
    m_alpha = alpha;
    m_color.set_alpha(alpha);
}