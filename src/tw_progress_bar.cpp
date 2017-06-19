#include "tw_progress_bar.h"

TWProgressBar::TWProgressBar(string level, double total_length){
    m_current_level = level;
    m_total_length = total_length;
    m_cur_length = 0;

    m_textures.push_back(resources::get_texture("whole-progress-bar.png"));
    m_textures.push_back(resources::get_texture("progress-bar.png"));
    m_textures.push_back(resources::get_texture("begin-progress-bar.png"));
    m_textures.push_back(resources::get_texture("tiny-will-progress-bar.png"));
}

TWProgressBar::~TWProgressBar(){
}

void TWProgressBar::update_current_length(double length){
    m_cur_length = length;
}

void TWProgressBar::update_self(unsigned, unsigned){
}

void TWProgressBar::draw_self(Canvas *canvas, unsigned, unsigned){
    double bar_width = 20 + (7.64 * 100 * m_cur_length) / m_total_length;
    double bar_x = 450;

    canvas->draw(m_textures[0].get(), Rectangle(0, 0, 800, 19), 26, bar_x);
    canvas->draw(m_textures[1].get(), Rectangle(0, 0, bar_width, 15), 30, bar_x + 2);
    canvas->draw(m_textures[2].get(), Rectangle(0, 0, 2, 15), 28, bar_x + 2);
    canvas->draw(m_textures[3].get(), Rectangle(0, 0, 20, 17), bar_width + 20, bar_x + 1);
}
