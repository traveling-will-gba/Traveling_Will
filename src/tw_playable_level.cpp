#include "tw_will.h"
#include "tw_playable_level.h"

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

TWPlayableLevel::TWPlayableLevel(const string &current_level, const string& next_level, const string audio_path, 
int audio_duration) : 
	m_is_punching(false), level_started(false), level_finished(false),
    m_audio_duration(audio_duration), m_audio_counter(0),
    m_punch_counter(0), n_collectables(0), n_enemies(0),
    m_cur_collectable_it(INVALID), m_cur_enemy_it(INVALID),
    m_x_speed(5/19.0), m_y_speed(0),
    sprite_counter(0), m_sprite_speed(1/170.0),
    m_camera_x(0), m_camera_y(0), m_reverse_camera_x(1), m_reverse_camera_y(480),
    m_cur_collectable(nullptr), m_cur_enemy(nullptr),
    m_number(resources::get_texture("numbers.png")){

    //printf("Entrando em construtor\n");

	m_current_level = current_level;
	m_audio = audio_path;
	m_next = next_level;
	m_done = false;
	m_state = RUNNING;
	m_audio_start = 0;
	m_start = -1;

	m_progress_bar[0] = resources::get_texture("whole-progress-bar.png");
	m_progress_bar[1] = resources::get_texture("progress-bar.png"); 
	m_progress_bar[2] = resources::get_texture("begin-progress-bar.png");
	m_will_progress_bar = resources::get_texture("tiny-will-progress-bar.png");

/*    m_background[0] = resources::get_texture(m_current_level + "/background_0.png");
    m_background[1] = resources::get_texture(m_current_level + "/background_1.png");
    m_background[2] = resources::get_texture(m_current_level + "/background_2.png");*/

    m_floor_texture = resources::get_texture(m_current_level + "/floor.png");

    m_collectable_icon = resources::get_texture(m_current_level + "/collectable_icon.png");

	//Read level design from txt
	fstream level_design("res/" + m_current_level + "/level_design.txt");

	if(not level_design.is_open()){
		//printf("Level design txt not available\n");
		exit(0);
	}

    level_design >> n_screens >> n_backgrounds;

    for(int i = 0; i < n_screens; ++i){
        int ph, e_present, c_present;
        int et = INVALID, eh = INVALID, ch = INVALID;

        level_design >> ph >> e_present;
        
        if(e_present){
            level_design >> et >> eh;
        }

        level_design >> c_present;

        if(c_present){
            level_design >> ch;
        }

        auto p = new TWPlatform(m_current_level, ph, et, eh, e_present, ch, c_present);
        platforms.push_back(p);
    }

    for(int i = 0; i < n_backgrounds; ++i){
        m_background[i] = resources::get_texture(m_current_level + "/background_" + to_string(i) + ".png");
    }

    for(int i = 0; i < NUMBER_OF_SECTIONS; ++i){
        auto p = platforms[i];
        p->set_x(i * PLATFORM_SIZE);
        add_child(p);
        p->register_objects(i * PLATFORM_SIZE);
    }

    level_design.close();
    
    //Sets initial will height based on level design
    m_floor = 480 - platforms[0]->height() - WILL_HEIGHT;

    m_will = new TWWill(50, 480 - platforms[0]->height() - WILL_HEIGHT);
    add_child(m_will);

    m_start = -1;

    event::register_listener(this);

    physics::set_collision_mode(physics::Mode::ONE_TO_ALL, m_will);

    //printf("Saindo de construtor\n");
}

TWPlayableLevel::~TWPlayableLevel(){
    event::unregister_listener(this);
}

bool TWPlayableLevel::done() const{
    return m_done;
}

string TWPlayableLevel::next() const{
    return m_next;
}

string TWPlayableLevel::audio() const{
    return m_audio;
}

bool TWPlayableLevel::on_event(const GameEvent&){
    //printf("Pegou evento\n");
	return false;
}

void TWPlayableLevel::update_self(unsigned now, unsigned last){
    //printf("Entrando em update_self\n");
    if(m_start == -1){
        m_start = now;
        m_audio_start = m_start;
    }

    update_counters(now);

    update_platforms_position();

    physics::do_collisions(now, last);

    test_floor(now);

    m_start = now;
    //printf("Saindo de update_self\n");
}

void TWPlayableLevel::test_floor(unsigned now){
    //printf("Entrando em test_floor\n");
    //Test TWWill colision

    //Start jump if TWWill is at the end of a cliff
    if(m_will->y() < m_floor && m_will->state() != JUMPING && m_will->state() != FALLING){
        m_will->set_state(FALLING);
        m_will->set_y_speed(0);
    }

    //Calculate jump speed and stop jump if hits the ground
    if(m_will->state() == JUMPING){
        m_will->update_y_speed((now - m_start)/300.0 * 0.5);

        if(m_will->speed() >= 0.001){
            m_will->set_state(FALLING);
        }
    }

    if(m_will->state() == FALLING){
        m_will->update_y_speed((now - m_start)/300.0 * 0.5);

        if(m_will->y() > m_floor){
            m_will->set_y(m_floor);
            m_will->set_y_speed(0);
            m_will->set_state(RUNNING);
        }
    }

    //printf("Saindo de test_floor\n");
}

void TWPlayableLevel::update_platforms_position(){
    //printf("Entrando em update_platforms_position\n");
    int height, current_x;

    for(int i = 0; i < NUMBER_OF_SECTIONS; ++i){
        platforms[i]->set_x(m_reverse_camera_x + PLATFORM_SIZE*i);
    }


    for(int i = 0; i < 2; ++i){
        current_x = platforms[i]->x();
        height = platforms[i]->height();

        if(current_x >= m_will->x() && current_x <= m_will->x() + WILL_WIDTH){
            m_floor = min(480.0 - height - WILL_HEIGHT, m_floor);
        }

        if(current_x >= m_will->x() && current_x <= m_will->x() + 30){
            m_floor = 480.0 - height - WILL_HEIGHT;
        }
    }

    if(m_will->y() >= m_floor + 5){
        m_floor = 430 - WILL_HEIGHT;
    }

    //printf("Saindo de update_platforms_position\n");
}

void TWPlayableLevel::update_counters(unsigned now){
    //printf("Entrando em update_counters\n");
    //Update counters based on time
    sprite_counter += (now - m_start) * m_sprite_speed;
    m_camera_x += (now - m_start) * m_x_speed;
    m_reverse_camera_x -= (now - m_start) * m_x_speed;

    if(not level_finished)
        m_audio_counter = now - m_audio_start;

    //Checking if music has ended
    if(m_audio_duration != -1 && m_audio_counter >= m_audio_duration){
        m_will->set_state(RUNNING);
        m_done = true;
    }

    //Reset value of reverse camera for each part of the level
    //printf("Entrando na treta\n");
    if(m_reverse_camera_x < -PLATFORM_SIZE && m_current_level == "1"){
        m_reverse_camera_x += PLATFORM_SIZE;
        destroy_child(platforms[0]);
        platforms.pop_front();
        platforms[NUMBER_OF_SECTIONS-1]->set_x(852);
        platforms[NUMBER_OF_SECTIONS-1]->register_objects(852);
        add_child(platforms[NUMBER_OF_SECTIONS-1]);
    }
    //printf("Saindo da treta\n");

	//Reset value of reverse camera for each part of the level
    if(m_reverse_camera_x < -142 && m_current_level == "2"){
        m_reverse_camera_x += 142;
        destroy_child(platforms[0]);
        platforms.pop_front();
        platforms[6]->set_x(852);
        platforms[6]->register_objects(852);
        add_child(platforms[6]);
    }

    printf("m_camera_x = %.2f\n", m_camera_x);

    //Reset background camera
    // if(m_camera_x > 1704){
    //     m_camera_x -= 1704;
    // }

    // Reset sprite counter
    if(sprite_counter > 5.9){
        sprite_counter -= 5.9;
    }
    //printf("Saindo de update_counters\n");
}

void TWPlayableLevel::draw_self(Canvas *canvas, unsigned, unsigned){
    //printf("Entrando em draw_self\n");
    canvas->clear();

    int divisor = 1 << (n_backgrounds - 1);
    for(int i = 0; i < n_backgrounds; ++i){
        //if(i != 1) continue;
        // if(i == 1 || i == 2) continue;
        canvas->draw(m_background[i].get(), Rectangle(fmod(m_camera_x/divisor, 1704), 0, 852, 480), 0, 0);
        //printf("divisor = %d\n", divisor);
        divisor /= 2;
    }

	// canvas->draw(m_background[0].get(), Rectangle(0, 0, 852, 480), 0, 0);
	// canvas->draw(m_background[1].get(), Rectangle(m_camera_x/2, m_camera_y, 852, 480), 0, 0);
	// canvas->draw(m_background[2].get(), Rectangle(m_camera_x, m_camera_y, 852, 480), 0, 0);

    double bar_width = 20 + (7.64 * 100 * m_audio_counter) / m_audio_duration;

    canvas->draw(m_progress_bar[0].get(), Rectangle(0, 0, 800, 19), 26, 18);
    canvas->draw(m_progress_bar[1].get(), Rectangle(0, 0, bar_width, 15), 30, 20);
    canvas->draw(m_progress_bar[2].get(), Rectangle(0, 0, 2, 15), 28, 20);
    canvas->draw(m_will_progress_bar.get(), Rectangle(0, 0, 20, 17), bar_width + 20, 20 - 1);

    canvas->draw(m_collectable_icon.get(), 705, 25);
    int x_digit = 805;
    int aux = n_collectables;
    do{
        canvas->draw(m_number.get(), Rectangle(23 * (aux % 10), 0, 23, 36), x_digit, 35);
        aux /= 10;
        x_digit -= 25;
    }while(aux);

    //printf("Saindo de draw_self\n");
}
