#include "CustomDrawingArea.hpp"
#include <cairomm/context.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <glibmm/fileutils.h>
#include <gtkmm/gesturedrag.h>
#include <iostream>

CustomDrawingArea::CustomDrawingArea(Glib::RefPtr<Gio::File> file)
    : m_file(nullptr),
      m_drag_status(Drag_Status::NONE),
      m_drag_x(0),
      m_drag_y(0),
      m_offset_x(0),
      m_offset_y(0),
      m_signal_to_save(false)
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

void CustomDrawingArea::on_draw(const Cairo::RefPtr<Cairo::Context> &cr, int width, int height)
{
    Gdk::Cairo::set_source_pixbuf(
        cr,
        m_image,
        (width - m_image->get_width()) / 2,
        (height - m_image->get_height()) / 2);
    cr->paint();
    for (auto i : m_changes)
    {
        draw_circle(cr, i.first, i.second);
    }
    if (m_signal_to_save)
    {
        cr->get_target()->write_to_png(m_file->get_path());
        m_signal_to_save = false;
    }
}

void CustomDrawingArea::draw_circle(const Cairo::RefPtr<Cairo::Context> &cr, int x, int y)
{
    cr->arc(x, y, 5.0, 0.0, 2.0 * M_PI); // full circle
    cr->fill();
    cr->set_source_rgba(0.337, 0.612, 0.117, 0.9);
    cr->stroke();
}

void CustomDrawingArea::on_drag_begin(double start_x, double start_y)
{
    m_drag_x = start_x;
    m_drag_y = start_y;
    m_changes.push_back(std::make_pair(start_x, start_y));
    queue_draw();
}

void CustomDrawingArea::on_drag_update(double offset_x, double offset_y)
{
    m_changes.push_back(std::make_pair(m_drag_x + offset_x, m_drag_y + offset_y));
    queue_draw();
}

void CustomDrawingArea::on_drag_end(double offset_x, double offset_y)
{
    m_changes.push_back(std::make_pair(m_drag_x + offset_x, m_drag_y + offset_y));
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