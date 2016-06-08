#include "platform.h"

Platform::Platform() : m_width(142) {

}

Platform::Platform(double ph, double et, double eh, int ep, double ch, int cp){
    m_width = 142;
    m_height = ph;
    m_y = 480 - m_height;
    m_texture = resources::get_texture("1/" + to_string((int)ph) + ".png");

    if(ep){
        m_enemy = new Enemy(et, eh);
        add_child(m_enemy);
    }else{
        m_enemy = nullptr;
    }

    if(cp){
        m_collectable = new Collectable(ch);
        add_child(m_collectable);
    }else{
        m_collectable = nullptr;
    }
}

Platform::~Platform(){
}

double Platform::x(){ return m_x; }
double Platform::y(){ return m_y; }
double Platform::height(){ return m_height; }
double Platform::width(){ return m_width; }
shared_ptr<Texture> Platform::texture(){ return m_texture; }
Collectable* Platform::collectable() { return m_collectable; }
Enemy* Platform::enemy() { return m_enemy; }

void Platform::set_x(double px){ m_x = px; }
void Platform::set_y(double py){  m_y = py; }
void Platform::set_height(double ph){ m_height = ph; }

void Platform::remove(int option){
    if(option == COLLECTABLE){
        remove_child(m_collectable);
        m_collectable = nullptr;
    }else{
        remove_child(m_enemy);
        m_enemy = nullptr;
    }
}

void Platform::register_objects(int m_x){
    if(m_collectable){
        m_collectable->register_self(m_x + 56);
    }
    if(m_enemy){
        m_enemy->register_self(m_x + 48);
    }
}

void Platform::update_self(unsigned, unsigned) {
    if(m_enemy){
         m_enemy->set_x(m_x + 48);
    }

    if(m_collectable){
        m_collectable->set_x(m_x + 56);
    }
}

void Platform::draw_self(Canvas * canvas, unsigned, unsigned) {
    canvas->draw(m_texture.get(), Rectangle(0, 0, m_width, m_height), m_x, m_y);
}

