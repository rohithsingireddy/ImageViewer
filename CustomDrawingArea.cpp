#include "CustomDrawingArea.hpp"
#include <cairomm/context.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <glibmm/fileutils.h>
#include <gtkmm/eventcontrollerkey.h>
#include <gtkmm/gesturedrag.h>
#include <iostream>

CustomDrawingArea::CustomDrawingArea(std::string fileName)
    : m_drag_status(Drag_Status::NONE),
      m_drag_x(0),
      m_drag_y(0),
      m_offset_x(0),
      m_offset_y(0)
{
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

    switch (m_drag_status)
    {
    case Drag_Status::BEGIN:
    {
        draw_circle(cr, m_drag_x, m_drag_y);
        break;
    }
    case Drag_Status::UPDATE:
    {
        draw_circle(cr, m_drag_x + m_offset_x, m_drag_y + m_offset_y);
        break;
    }
    case Drag_Status::END:
    {
        draw_circle(cr, m_drag_x + m_offset_x, m_drag_y + m_offset_y);
        m_drag_status = Drag_Status::NONE;
        break;
    }
    default:
    {
        // m_drag_x = 0;
        // m_drag_y = 0;
        // m_offset_x = 0;
        // m_offset_y = 0;
        break;
    }
    }
}

void CustomDrawingArea::draw_circle(const Cairo::RefPtr<Cairo::Context> &cr, int x, int y)
{ 
    cr->arc(x, y, 1.0, 0.0, 2.0 * M_PI); // full circle
    cr->fill_preserve();
    cr->set_source_rgba(0.337, 0.612, 0.117, 0.9);
    cr->stroke_preserve();
}

void CustomDrawingArea::on_drag_begin(double start_x, double start_y)
{
    m_drag_x = start_x;
    m_drag_y = start_y;
    m_drag_status = Drag_Status::BEGIN;
    queue_draw();
}

void CustomDrawingArea::on_drag_update(double offset_x, double offset_y)
{
    m_offset_x = offset_x;
    m_offset_y = offset_y;
    m_drag_status = Drag_Status::UPDATE;
    queue_draw();
}

void CustomDrawingArea::on_drag_end(double offset_x, double offset_y)
{
    m_offset_x = offset_x;
    m_offset_y = offset_y;
    m_drag_status = Drag_Status::END;
    queue_draw();
}