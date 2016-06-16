#include "tw_button.h"

#include <ijengine/canvas.h>

TWButton::TWButton(int b_id, string cur_level, double b_x, double b_y, string img, double b_w, double b_h, int able_draw) :
    m_id(b_id), m_able_draw(able_draw), m_img(img), m_level(cur_level), m_x(b_x), m_y(b_y), m_img_h(b_h), m_img_w(b_w){
    
    m_texture = resources::get_texture(cur_level + "/" + img);
}

TWButton::~TWButton(){
}

// 1 if able to draw, 0 otherwise
void TWButton::set_able_to_draw(int num){
    m_able_draw = num;
}

int TWButton::able_to_draw(){ return m_able_draw; }
double TWButton::x(){ return m_x; }
double TWButton::y(){ return m_y; }
double TWButton::w(){ return m_img_w; }
double TWButton::h(){ return m_img_h; }
string TWButton::level(){ return m_level; }
int TWButton::id(){ return m_id; }

void TWButton::update_self(unsigned, unsigned){
}

void TWButton::draw_self(Canvas *canvas, unsigned, unsigned){
    if(m_able_draw == 1){
        canvas->draw(m_texture.get(), m_x, m_y);
    }
}
