#include "CustomScrollableWindow.hpp"

CustomScrollableWindow::CustomScrollableWindow(CustomDrawingArea *area)
    : m_drawing_area(nullptr)
{
    m_drawing_area = area;
}

CustomDrawingArea *CustomScrollableWindow::get_drawing_area()
{
    return m_drawing_area;
}