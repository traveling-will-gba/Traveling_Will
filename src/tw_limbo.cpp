#include "tw_limbo.h"
#include "tw_will.h"
#include "tw_portal_to_level.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/keyboard_event.h>

#include <unistd.h>
#include <cmath>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace ijengine;

TWLimbo::TWLimbo(const string &current_level, const string& next_level, const string audio_path, 
int audio_duration) : 
    m_audio_duration(audio_duration), m_audio_counter(0), sprite_counter(0), m_sprite_speed(1/170.0){

    m_current_level = current_level;
    m_audio = audio_path;
    m_next = next_level;
    m_done = false;
    m_state = RUNNING;
    m_audio_start = 0;
    m_start = -1;

    on_portal = false;

    m_background_texture = resources::get_texture(m_current_level + "/background.png");
    
    m_will = new TWWill(100, 5);
    m_will->set_m_active_events(false);
    add_child(m_will);

	int n_levels = 6;
	m_save = new TWSave(n_levels);

	pair <int, int> portals_pos[] = {   make_pair(0, 0), make_pair(100, 5), make_pair(480, 0),
                                        make_pair(615, 135), make_pair(285, 140),
                                        make_pair(330, 280), make_pair(130, 340)};

	int n_unlocked_portals = 0;

	for(int i = 1; i <= n_levels; i++){
		if(m_save->is_unlocked(i)){
			m_portal[i] = new TWPortalToLevel(to_string(i), portals_pos[i].first, portals_pos[i].second, 1, m_save->record_collectables(i));

			n_unlocked_portals++;
		}
	}

    for(int i = 1; i <= n_unlocked_portals; i++){
        add_child(m_portal[i]);
    }

    m_buttons.clear();
    m_buttons.push_back(new TWButton("back", m_current_level, 30, 410, "voltar-botao.png", 142, 50));

    for(auto btn : m_buttons)
        add_child(btn);

    m_start = -1;

    event::register_listener(this);

    physics::set_collision_mode(physics::Mode::ONE_TO_ALL, m_will);

    //printf("Saindo de construtor\n");
}

TWLimbo::~TWLimbo(){
    event::unregister_listener(this);
}

void TWLimbo::set_next(string next_level){
    m_next = next_level;
}

bool TWLimbo::done() const{
    return m_done;
}

string TWLimbo::next() const{
    return m_next;
}

string TWLimbo::audio() const{
    return m_audio;
}

void TWLimbo::do_action(string label){
    if(label == "back"){
        m_will->set_active(false);
        m_next = "menu";
        m_done = true;
    }
}

bool TWLimbo::on_event(const GameEvent& event){
    if(event.id() == GAME_EVENT_DOWN_PRESSED){
        m_will->set_y_speed(0.15);
    }

    if(event.id() == GAME_EVENT_DOWN_RELEASED){
        m_will->set_y_speed(0);
    }

    if(event.id() == GAME_EVENT_UP_PRESSED){
        m_will->set_y_speed(-0.15);
    }

    if(event.id() == GAME_EVENT_UP_RELEASED){
        m_will->set_y_speed(0);
    }

    if(event.id() == GAME_EVENT_LEFT_PRESSED){
        m_will->set_x_speed(-0.15);
    }

    if(event.id() == GAME_EVENT_LEFT_RELEASED){
        m_will->set_x_speed(0);
    }

    if(event.id() == GAME_EVENT_RIGHT_PRESSED){
        m_will->set_x_speed(0.15);
    }

    if(event.id() == GAME_EVENT_RIGHT_RELEASED){
        m_will->set_x_speed(0);
    }

    if(event.id() == GAME_EVENT_ENTER){
        if(m_next != ""){
            m_done = true;
        }
    }

    return true;
}

void TWLimbo::update_self(unsigned now, unsigned last){
    if(m_will->x_speed() || m_will->speed()) m_will->set_state(RUNNING);
    else m_will->set_state(STOPPED);

    sprite_counter += (now - m_start) * m_sprite_speed;

    // Reset sprite counter
    if(sprite_counter > 5.9){
        sprite_counter -= 5.9;
    }

    if(m_start == -1){
        m_start = now;
        m_audio_start = m_start;
    }

    physics::do_collisions(now, last);

    on_portal = false;

    m_start = now;
}  

void TWLimbo::draw_self(Canvas *canvas, unsigned, unsigned){
    canvas->clear();

    canvas->draw(m_background_texture.get(), Rectangle(0, 0, 852, 480), 0, 0);
}
