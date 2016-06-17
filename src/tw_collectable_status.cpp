#include "tw_collectable_status.h"

TWCollectableStatus::TWCollectableStatus(string level){
    m_current_level = level;
    m_collectable_counter = 0;

    m_textures.push_back(resources::get_texture(m_current_level + "/collectable_icon.png"));
    m_textures.push_back(resources::get_texture("numbers.png"));
}

TWCollectableStatus::~TWCollectableStatus(){
}

void TWCollectableStatus::update_collectable_counter(int counter){
    m_collectable_counter = counter;
}

void TWCollectableStatus::update_self(unsigned, unsigned){
}

void TWCollectableStatus::draw_self(Canvas *canvas, unsigned, unsigned){
    canvas->draw(m_textures[0].get(), 680, 400);

    int x_digit = 780;
    int counter = m_collectable_counter;

    do{
        canvas->draw(m_textures[1].get(), Rectangle(23 * (counter % 10), 0, 23, 36), x_digit, 410);
        counter /= 10;
        x_digit -= 25;
    }while(counter);
}
