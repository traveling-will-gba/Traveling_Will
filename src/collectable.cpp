#include "collectable.h"

Collectable::Collectable(){

}

Collectable::Collectable(double ch, int cp){
    m_height = m_width = 30;
    m_y = 480 - ch;
    m_present = cp;
    m_sprite = resources::get_texture("1/collectable.png");
}

Collectable::~Collectable(){

}

double Collectable::x(){ return m_x; }
double Collectable::y(){ return m_y; }
double Collectable::height(){ return m_height; }
double Collectable::width(){ return m_width; }
int Collectable::present(){ return m_present; }
shared_ptr<Texture> Collectable::texture(){ return m_sprite; }

void Collectable::set_x(double cx) { m_x = cx; }
void Collectable::set_y(double cy) { m_y = cy; }
void Collectable::set_height(double ch) { m_height = ch; }
void Collectable::set_present(int cp) { m_present = cp; }

void Collectable::update_self(unsigned, unsigned) {}
void Collectable::draw_self(Canvas*, unsigned, unsigned) {}

