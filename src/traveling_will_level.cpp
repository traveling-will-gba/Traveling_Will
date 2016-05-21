#include "traveling_will_level.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/keyboard_event.h>

#include <unistd.h>

#include <iostream>

using namespace std;
using namespace ijengine;

static const int GAME_EVENT_JUMP =              1 << 4;
static const int GAME_EVENT_SLIDE_PRESSED =     1 << 5;
static const int GAME_EVENT_SLIDE_RELEASED =    1 << 6;
static const int GAME_EVENT_MENU_SELECT =       1 << 7;
static const int NUMBER_OF_SCREENS = 2;

TravelingWillLevel::TravelingWillLevel(int r, int g, int b, const string &current_level, const string& next_level, const string audio_path)
    : m_r(r), m_g(g), m_b(b), m_done(false), m_next(next_level), m_start(-1), m_camera_x(0),
    m_y_speed(0), m_current_level(current_level), m_audio(audio_path), m_state(NOTHING){

        printf("current_level: [%s]\n", m_current_level.c_str());
        printf("Audio of level [%s]\n", m_audio.c_str());

        if(m_current_level == "menu"){
            m_camera_y = 0;
            printf("[%s]\n", (m_current_level + "/Menu.png").c_str());
            m_background[0] = resources::get_texture(m_current_level + "/Menu.png");
        }
        else if(m_current_level == "1"){
            m_state = RUNNING;
            m_will = resources::get_texture(m_current_level + "/will.png");
            m_camera_y = 0;
            m_will_x = 50;
            m_will_y = 293;
            m_boss_x = 690;
            m_boss_y = 190;
            m_background[0] = resources::get_texture(m_current_level + "/background_floresta_0.png");
            m_background[1] = resources::get_texture(m_current_level + "/background_floresta_1.png");
            m_background[2] = resources::get_texture(m_current_level + "/background_floresta_2.png");

            current_image = 0;
            for(int i = 0; i < NUMBER_OF_SCREENS; ++i){
            	level_image_path[i] = m_current_level + "/level_" + to_string(i) + ".png";
           	}

            m_level[0] = resources::get_texture(m_current_level + "/level_0.png");
            m_level[1] = resources::get_texture(m_current_level + "/level_1.png");
            m_boss = resources::get_texture(m_current_level + "/capetinha_voador.png");
            m_x_speed = 4000/19000.0;
        }

        event::register_listener(this);
}

TravelingWillLevel::~TravelingWillLevel(){
    event::unregister_listener(this);
}

bool TravelingWillLevel::done() const{
    return m_done;
}

string TravelingWillLevel::next() const{
    return m_next;
}

string TravelingWillLevel::audio() const{
    return m_audio;
}

bool TravelingWillLevel::on_event(const GameEvent& event){
    if(event.id() == GAME_EVENT_MENU_SELECT && m_state == NOTHING){
        m_state = SELECTING;
        return true;
    }

    if(event.id() == GAME_EVENT_JUMP && m_state == RUNNING){
        m_y_speed = -0.5;
        m_state = JUMPING;
        return true;
    }

    if(event.id() == GAME_EVENT_SLIDE_PRESSED && m_state != JUMPING){
        m_will = resources::get_texture("will-slide.png");
        m_state = SLIDING;
        return true;
    }

    if(event.id() == GAME_EVENT_SLIDE_RELEASED && m_state == SLIDING){
        m_will = resources::get_texture("will.png");
        m_state = RUNNING;
        return true;
    }

    return true;
}

void TravelingWillLevel::update_self(unsigned now, unsigned){
    if(m_start == -1)
        m_start = now;

    printf("ms: %d ",now);

    if(m_state == NOTHING) printf("NOTHING\n");
    if(m_state == RUNNING) printf("RUNNING\n");
    if(m_state == JUMPING) printf("JUMPING\n");
    if(m_state == SELECTING) printf("SELECTING\n");
    if(m_state == SLIDING) printf("SLIDING\n");

    if(m_state == SELECTING){
        m_state = RUNNING;
        m_done = true;
    }

    if(m_camera_x > 852){
        double diff = m_camera_x - 852;
        m_camera_x = diff;

        current_image = (current_image + 1)%NUMBER_OF_SCREENS;
        int next_image = (current_image + 1)%NUMBER_OF_SCREENS;
        m_level[0] = resources::get_texture(level_image_path[current_image]);
        m_level[1] = resources::get_texture(level_image_path[next_image]);
    }

    if(m_state == JUMPING){
        m_y_speed += (now - m_start)/300.0 * 0.5;

        if(m_will_y + (now - m_start) * m_y_speed > 293){
            m_will_y = 293;
            m_boss_y = 190;
            m_y_speed = 0;
            m_state = RUNNING;
        }
    }

    m_camera_x += (now - m_start) * m_x_speed;
    m_will_y += (now - m_start) * m_y_speed;

    m_start = now;
}

void TravelingWillLevel::draw_self(Canvas *canvas, unsigned, unsigned){
    canvas->clear();
    canvas->draw(m_background[0].get(), Rectangle(0, 0, 854, 480), 0, 0);

    if(m_current_level != "menu"){
        canvas->draw(m_background[1].get(), Rectangle(m_camera_x/2, m_camera_y, 854, 480), 0, 0);
        canvas->draw(m_background[2].get(), Rectangle(m_camera_x, m_camera_y, 854, 480), 0, 0);
        canvas->draw(m_level[0].get(), Rectangle(m_camera_x, m_camera_y, 854, 480), 0, 0);
        canvas->draw(m_will.get(), m_will_x, m_will_y);
        canvas->draw(m_boss.get(), m_boss_x, m_boss_y);
    }
}
