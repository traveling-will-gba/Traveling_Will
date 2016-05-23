#include "button.h"

#include <ijengine/canvas.h>

Button::Button(int b_id, string level, double b_x, double b_y, string img, double b_w, double b_h) :
    m_id(b_id), m_img(img), m_level(level), m_x(b_x), m_y(b_y), m_img_h(b_h), m_img_w(b_w){
    
    m_texture = resources::get_texture(level + "/" + img);
}

Button::~Button(){

}

double Button::x(){
    return m_x;
}

double Button::y(){
    return m_y;
}

double Button::w(){
    return m_img_w;
}

double Button::h(){
    return m_img_h;
}

int Button::id(){
    return m_id;
}

void Button::update_self(unsigned, unsigned){
}

void Button::draw_self(Canvas *canvas, unsigned, unsigned){
    canvas->draw(m_texture.get(), m_x, m_y);
}
