#include "tw_progress_bar.h"

TWProgressBar::TWProgressBar(string level, int audio_duration){
    m_current_level = level;
    m_audio_duration = audio_duration;
    m_audio_counter = 0;

    m_textures.push_back(resources::get_texture("whole-progress-bar.png"));
    m_textures.push_back(resources::get_texture("progress-bar.png"));
    m_textures.push_back(resources::get_texture("begin-progress-bar.png"));
    m_textures.push_back(resources::get_texture("tiny-will-progress-bar.png"));
}

TWProgressBar::~TWProgressBar(){
}

void TWProgressBar::update_audio_counter(int audio){
    m_audio_counter = audio;
}

void TWProgressBar::update_self(unsigned, unsigned){
}

void TWProgressBar::draw_self(Canvas *canvas, unsigned, unsigned){
    double bar_width = 20 + (7.64 * 100 * m_audio_counter) / m_audio_duration;

    canvas->draw(m_textures[0].get(), Rectangle(0, 0, 800, 19), 26, 18);
    canvas->draw(m_textures[1].get(), Rectangle(0, 0, bar_width, 15), 30, 20);
    canvas->draw(m_textures[2].get(), Rectangle(0, 0, 2, 15), 28, 20);
    canvas->draw(m_textures[3].get(), Rectangle(0, 0, 20, 17), bar_width + 20, 20 - 1);
}
