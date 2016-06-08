#include "collectable.h"

Collectable::Collectable(){

}

Collectable::Collectable(double ch){
    m_height = m_width = 30;
    m_y = 480 - ch;
    m_texture = resources::get_texture("1/collectable.png");
}

Collectable::~Collectable(){

}

double Collectable::x(){ return m_x; }
double Collectable::y(){ return m_y; }
double Collectable::height(){ return m_height; }
double Collectable::width(){ return m_width; }
shared_ptr<Texture> Collectable::texture(){ return m_texture; }

void Collectable::set_x(double cx) { m_x = cx; }
void Collectable::set_y(double cy) { m_y = cy; }
void Collectable::set_height(double ch) { m_height = ch; }

void Collectable::update_self(unsigned, unsigned) {}
void Collectable::draw_self(Canvas* canvas, unsigned, unsigned) {
    canvas->draw(m_texture.get(), Rectangle(m_width * 0, 0, m_width, m_height), m_x, m_y);
}

