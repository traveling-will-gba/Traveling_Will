#include "tw_button.h"
#include "tw_menu.h"

#include <ijengine/canvas.h>

TWButton::TWButton(string btn_label, string cur_level, double b_x, double b_y, string img, double b_w, double b_h) :
    m_label(btn_label), m_img(img), m_level(cur_level), m_x(b_x),
    m_y(b_y), m_h(b_h), m_w(b_w), m_state(NOT_CLICKING) {
    
    m_texture_label = img;
    m_texture = resources::get_texture(cur_level + "/" + m_texture_label);

    event::register_listener(this);
}

TWButton::~TWButton(){
    event::unregister_listener(this);
}

double TWButton::x(){ return m_x; }
double TWButton::y(){ return m_y; }
double TWButton::w(){ return m_w; }
double TWButton::h(){ return m_h; }
string TWButton::level(){ return m_level; }
string TWButton::label(){ return m_label; }
string TWButton::texture(){ return m_texture_label; }

void TWButton::set_texture(string texture){
    m_texture = resources::get_texture(m_level + "/" + texture);
}

void TWButton::update_self(unsigned, unsigned){}

bool TWButton::on_event(const GameEvent& event){
    if(event.id() == GAME_MOUSE_CLICK){
        double mouse_x = event.get_property<double>("x");
        double mouse_y = event.get_property<double>("y");

        int min_x = m_x, max_x = m_x + m_w;
        int min_y = m_y, max_y = m_y + m_h;

        if(mouse_x >= min_x && mouse_x <= max_x && mouse_y >= min_y && mouse_y <= max_y){
            if(m_state == CLICKING){
                auto p = this->parent();

                if(p){
                    auto menu = dynamic_cast<TWMenu *>(p);
                    menu->do_action(m_label);
                }

                m_state = NOT_CLICKING;
                return true;
            }

            m_state = CLICKING;
            return true;
        }
    }

    return false;
}

void TWButton::draw_self(Canvas *canvas, unsigned, unsigned){
    canvas->draw(m_texture.get(), m_x, m_y);
}
