#include "traveling_will_level.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/keyboard_event.h>

#include <unistd.h>

using namespace std;
using namespace ijengine;

static const int GAME_EVENT_JUMP = 16;

TravelingWillLevel::TravelingWillLevel(int r, int g, int b, const string &current_level, const string& next_level)
    : m_r(r), m_g(g), m_b(b), m_done(false), m_next(next_level), m_start(-1),
    m_camera_x(0), m_camera_y(80),
    m_y_speed(0), m_state(RUNNING){

	m_current_level = current_level;

	printf("%s\n", current_level.c_str());

	if(current_level == "menu"){
		m_texture = resources::get_texture("Menu.png");
	}else if(current_level == "1"){
		m_will = resources::get_texture("dino.png");
		m_texture = resources::get_texture("test2.png");
		m_x_speed = 4000/19000.0;

		m_translator.add_translation(KeyboardEvent(0, KeyboardEvent::PRESSED, KeyboardEvent::SPACE, KeyboardEvent::NONE), GameEvent(GAME_EVENT_JUMP));
		event::register_translator(&m_translator);
		event::register_listener(this);
	}
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

    return false;
}

string TravelingWillLevel::next() const{
	sleep(1);
    return m_next;
}

void TravelingWillLevel::update_self(unsigned now, unsigned){
    if(m_start == -1)
        m_start = now;

	if(m_current_level == "menu")
		m_done = true;

    if(m_camera_x > 1200)
        m_done = true;

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

	if(m_current_level != "menu")
	    canvas->draw(m_will.get(), 50, 260);
}
