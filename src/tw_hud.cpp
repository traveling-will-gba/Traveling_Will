#include "tw_hud.h"

TWHud::TWHud(string level, int type){
    m_current_level = level;
    m_type = type;

    if(m_type == COLLECTABLES){
        shared_ptr<Texture> texture;
        texture = resources::get_texture(m_current_level + "/collectable_icon.png");
        m_textures.push_back(texture);
        texture = resources::get_texture("numbers.png");
        m_textures.push_back(texture);
    }
    else if(m_type == PROGRESS_BAR){
        shared_ptr<Texture> texture;
        texture = resources::get_texture("whole-progress-bar.png");
        m_textures.push_back(texture);
        texture = resources::get_texture("progress-bar.png"); 
        m_textures.push_back(texture);
        texture = resources::get_texture("begin-progress-bar.png");
        m_textures.push_back(texture);
        texture = resources::get_texture("tiny-will-progress-bar.png");
        m_textures.push_back(texture);
    }
}

void TWHud::set_attribute(string name, double value){
    m_attributes[name] = value;
}

void TWHud::update_self(unsigned, unsigned){

}

void TWHud::draw_self(Canvas *canvas, unsigned, unsigned){
    if(m_type == COLLECTABLES){
        canvas->draw(m_textures[0].get(), 705, 25);

        int x_digit = 805;
        int coll_number = (int) m_attributes["collectables"];

        do{
            canvas->draw(m_textures[1].get(), Rectangle(23 * (coll_number % 10), 0, 23, 36), x_digit, 35);
            coll_number /= 10;
            x_digit -= 25;
        }while(coll_number);
    }
    else if(m_type == PROGRESS_BAR){
        int audio_counter = (int) m_attributes["audio_counter"];
        int audio_duration = (int) m_attributes["audio_duration"];

        double bar_width = 20 + (7.64 * 100 * audio_counter) / audio_duration;

        canvas->draw(m_textures[0].get(), Rectangle(0, 0, 800, 19), 26, 18);
        canvas->draw(m_textures[1].get(), Rectangle(0, 0, bar_width, 15), 30, 20);
        canvas->draw(m_textures[2].get(), Rectangle(0, 0, 2, 15), 28, 20);
        canvas->draw(m_textures[3].get(), Rectangle(0, 0, 20, 17), bar_width + 20, 20 - 1);
    }
}
