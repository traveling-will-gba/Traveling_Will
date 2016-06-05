#include "collectable.h"

Collectable::Collectable(){

}

Collectable::Collectable(double cy, double ch){

}

Collectable::~Collectable(){

}

double Collectable::x(){ return m_x; }
double Collectable::y(){ return m_y; }
double Collectable::height(){ return m_height; }
double Collectable::width(){ return m_width; }
void Collectable::set_x(double cx) { m_x = cx; }
void Collectable::set_y(double cy) { m_y = cy; }

void Collectable::update_self(unsigned now, unsigned last){

}

void Collectable::draw_self(Canvas *canvas, unsigned now, unsigned last){
    canvas->draw(m_sprite.get(), m_x, m_y);
}

