#include "tw_tutorial.h"

TWTutorial::TWTutorial(string t_label, string texture_path, bool t_active){
    m_label = t_label;
    m_active = t_active;

    m_textures.push_back(resources::get_texture(texture_path));
}

TWTutorial::~TWTutorial(){
}

string TWTutorial::label(){ return m_label; }
bool TWTutorial::active(){ return m_active; }
void TWTutorial::set_active(bool act){ m_active = act; }

void TWTutorial::update_self(unsigned, unsigned){
}

void TWTutorial::draw_self(Canvas *canvas, unsigned, unsigned){
    if(m_active)
        canvas->draw(m_textures[0].get(), 20, 50);
}
