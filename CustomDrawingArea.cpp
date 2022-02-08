#include "CustomDrawingArea.hpp"
#include <cairomm/context.h>
#include <gdkmm/general.h> // set_source_pixbuf()
#include <glibmm/fileutils.h>
#include <gtkmm/eventcontrollerkey.h>
#include <iostream>

CustomDrawingArea::CustomDrawingArea(std::string fileName)
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
}