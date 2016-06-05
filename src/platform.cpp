#include "platform.h"

Platform::Platform() : m_width(142) {

}

Platform::Platform(double ph, double et, double eh, int ep, double ch, int cp){
    m_width = 142;
    m_height = ph;
    m_y = 480 - m_height;
    m_sprite = resources::get_texture("1/" + to_string((int)ph) + ".png");

    m_enemy = new Enemy(et, eh, ep);
    add_child(m_enemy);

    m_collectable = new Collectable(ch, cp);
    add_child(m_collectable);
}

Platform::~Platform(){
}

double Platform::x(){ return m_x; }
double Platform::y(){ return m_y; }
double Platform::height(){ return m_height; }
double Platform::width(){ return m_width; }
shared_ptr<Texture> Platform::texture(){ return m_sprite; }
Collectable* Platform::collectable() { return m_collectable ? m_collectable : nullptr; }
Enemy* Platform::enemy() { return m_enemy ? m_enemy : nullptr; }

void Platform::set_x(double px){ m_x = px; }
void Platform::set_y(double py){  m_y = py; }
void Platform::set_height(double ph){ m_height = ph; }

void Platform::update_self(unsigned, unsigned) {}

void Platform::draw_self(Canvas*, unsigned, unsigned) {}

