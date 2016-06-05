#include "platform.h"

Platform::Platform(){

}

Platform::~Platform(){

}

double Platform::x(){ return m_x; }
double Platform::y(){ return m_y; }
double Platform::height(){ return m_height; }
double Platform::widht(){ return m_width; }
void Platform::set_x(double px){ m_x = px; }
void Platform::set_y(double py){  m_y = py; }

void Platform::update_self(unsigned now, unsigned last){

}

void Platform::draw_self(Canvas *canvas, unsigned now, unsigned last){
    canvas->draw(m_sprite.get(), m_x, m_y);
}

