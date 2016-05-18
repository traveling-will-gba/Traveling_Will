#include "traveling_will_level.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/keyboard_event.h>

using namespace std;
using namespace ijengine;

static const int GAME_EVENT_JUMP = 16;
static const int GAME_EVENT_SLIDE_PRESSED = 32;
static const int GAME_EVENT_SLIDE_RELEASED = 64;

TravelingWillLevel::TravelingWillLevel(int r, int g, int b, const string& next_level)
    : m_r(r), m_g(g), m_b(b), m_done(false), m_next(next_level), m_start(-1),
    m_camera_x(0), m_camera_y(80), m_will_x(50), m_will_y(270),
    m_x_speed(4000 / 19000.0), m_y_speed(0), m_state(RUNNING),
    m_texture(resources::get_texture("background.png")), m_will(resources::get_texture("will.png")) {

    m_translator.add_translation(KeyboardEvent(0, KeyboardEvent::PRESSED, KeyboardEvent::SPACE, KeyboardEvent::NONE), GameEvent(GAME_EVENT_JUMP));
    m_translator.add_translation(KeyboardEvent(0, KeyboardEvent::PRESSED, KeyboardEvent::DOWN, KeyboardEvent::NONE), GameEvent(GAME_EVENT_SLIDE_PRESSED));
    m_translator.add_translation(KeyboardEvent(0, KeyboardEvent::RELEASED, KeyboardEvent::DOWN, KeyboardEvent::NONE), GameEvent(GAME_EVENT_SLIDE_RELEASED));
    event::register_translator(&m_translator);
    event::register_listener(this);
}

TravelingWillLevel::~TravelingWillLevel(){
    event::unregister_listener(this);
    event::unregister_translator(&m_translator);
}

bool TravelingWillLevel::done() const{
    return m_done;
}

bool TravelingWillLevel::on_event(const GameEvent& event){
    if(event.type() == GAME_EVENT_JUMP && m_state != JUMPING){
        m_y_speed = -0.5;
        m_state = JUMPING;
        return true;
    }

    if(event.type() == GAME_EVENT_SLIDE_PRESSED){
        m_will = resources::get_texture("will-slide.png");
        m_state = SLIDING;
        m_will_y = 300;
        return true;
    }

    if(event.type() == GAME_EVENT_SLIDE_RELEASED){
        m_will = resources::get_texture("will.png");
        m_state = RUNNING;
        m_will_y = 270;
        return true;
    }

    return false;
}

string TravelingWillLevel::next() const{
    return m_next;
}

void TravelingWillLevel::update_self(unsigned now, unsigned){
    if(m_start == -1)
        m_start = now;

    if(m_camera_x > 1200 && m_camera_x < 1300)
        m_camera_x = 0;

//    if(music.finished())
//        m_done = true;

    if(m_state == JUMPING){
        m_y_speed += (now - m_start)/300.0 * 0.5;

        if(m_camera_y + (now - m_start) * m_y_speed > 80){
            m_camera_y = 80;
            m_y_speed = 0;
            m_state = RUNNING;
        }
    }

    m_camera_x += (now - m_start) * m_x_speed;
    m_camera_y += (now - m_start) * m_y_speed;

    printf("state = %d, cx = %.2f, cy = %.2f\n", m_state, m_camera_x, m_camera_y);
    m_start = now;
}

void TravelingWillLevel::draw_self(Canvas *canvas, unsigned, unsigned){
    canvas->clear();
    canvas->draw(m_texture.get(), Rectangle(m_camera_x, m_camera_y, 854, 480), 0, 0);
    canvas->draw(m_will.get(), m_will_x, m_will_y);
}
