#include "traveling_will_playable_level.h"
#include "will.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/keyboard_event.h>

#include <unistd.h>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace ijengine;

TravelingWillPlayableLevel::TravelingWillPlayableLevel(const string &current_level, const string& next_level, const string audio_path, 
int audio_duration) : 
	m_is_punching(false), level_started(false), level_finished(false),
    m_audio_duration(audio_duration), m_audio_counter(0),
    n_collectables(0), n_enemies(0),
    collectable_it(-10000000), enemy_it(-10000000), m_enemy_type(-1), m_punch_counter(0),
    m_x_speed(5/19.0), m_y_speed(0),
    sprite_counter(0), m_sprite_speed(1/170.0),
    m_camera_x(0), m_camera_y(0), m_reverse_camera_x(1), m_reverse_camera_y(480),
    m_will_x(50), m_collectable(-100), m_enemy(-100),m_number(resources::get_texture("numbers.png")){

    printf("Entrando em construtor\n");

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

    m_background[0] = resources::get_texture(m_current_level + "/background_0.png");
    m_background[1] = resources::get_texture(m_current_level + "/background_1.png");
    m_background[2] = resources::get_texture(m_current_level + "/background_2.png");

    m_enemy_texture[0] = resources::get_texture(m_current_level + "/enemy1.png");
    m_enemy_texture[1] = resources::get_texture(m_current_level + "/enemy2.png");

    m_collectable_texture = resources::get_texture(m_current_level + "/collectable.png");
    m_collectable_icon = resources::get_texture(m_current_level + "/collectable_icon.png");

	//Read level design from txt
	fstream level_design("res/" + m_current_level + "/level_design.txt");
	if(not level_design.is_open()){
		printf("Level design txt not available\n");
		exit(0);
	}
	level_design >> n_screens;

    platform_height.resize(n_screens + 1);
    collectable_height.resize(n_screens + 1);
    enemy_height.resize(n_screens + 1);

    collectable.resize(n_screens + 1);
    enemy.resize(n_screens + 1);
    enemy_type.resize(n_screens + 1);

    for(int i = 0; i < n_screens; ++i){
        // int p_h;
        // level_design >> p_h;
        // platforms[i].set_height(p_h);

        level_design >> platform_height[i];
        level_design >> enemy[i];
        
        if(enemy[i]){
            level_design >> enemy_type[i];
            level_design >> enemy_height[i];
        } 
        
        level_design >> collectable[i];
        if(collectable[i]){
            level_design >> collectable_height[i];
        }
        
        // printf("platform_height[%d] = %.2f\n", i, platforms[i].height);
        printf("platform_height[%d] = %.2f\n", i, platform_height[i]);
    }

    level_design.close();
    
    //Sets initial will height based on level design
    m_floor = 480 - platform_height[0] - WILL_HEIGHT;

    //Get platforms textures
    for(int i = 1; i < 9; i++){
        m_level[i] = resources::get_texture(m_current_level + "/" + to_string(i*50) + ".png");
    }

    m_will = new Will(50, 480 - platform_height[0] - WILL_HEIGHT);
    add_child(m_will);

    m_start = -1;

	event::register_listener(this);
    printf("Saindo de construtor\n");
}

TravelingWillPlayableLevel::~TravelingWillPlayableLevel(){
    event::unregister_listener(this);
}

bool TravelingWillPlayableLevel::done() const{
    return m_done;
}

string TravelingWillPlayableLevel::next() const{
    return m_next;
}

string TravelingWillPlayableLevel::audio() const{
    return m_audio;
}

bool TravelingWillPlayableLevel::on_event(const GameEvent&){
	return false;
}

void TravelingWillPlayableLevel::update_self(unsigned now, unsigned){
    printf("Entrando em update_self\n");
    if(m_start == -1){
        m_start = now;
        m_audio_start = m_start;
    }

    update_counters(now);

    update_platforms_position();

    do_collisions(now);

    check_game_over();

    m_start = now;
    printf("Saindo de update_self\n");
}

void TravelingWillPlayableLevel::do_collisions(unsigned now){
    printf("Entrando em do_collisions\n");
    //Test Will colision
    if(m_will->y() > m_floor + 20){
        m_will->set_state(GAME_OVER);
    }

    //Start jump if Will is at the end of a cliff
    if(m_will->y() < m_floor && m_will->state() != JUMPING && m_will->state() != FALLING && m_will->state() != GAME_OVER){
        m_will->set_state(FALLING);
        m_will->set_y_speed(0);
    }

    //Calculate jump speed and stop jump if hits the ground
    if(m_will->state() == JUMPING || m_will->state() == FALLING){
        m_will->update_y_speed((now - m_start)/300.0 * 0.5);

        if(m_will->speed() >= 0.001){
            m_will->set_state(FALLING);
        }

        if(m_will->y() > m_floor){
            m_will->set_y(m_floor);
            m_will->set_y_speed(0);
            m_will->set_state(RUNNING);
        }
    }

    if(m_will->y() >= m_collectable && m_will->y() + 15*(m_will->state() == SLIDING ? 1 : 0) <= m_collectable + COLLECTABLE_SIZE){
        ++n_collectables;

        collectable[collectable_it] = 0;
        m_collectable = INVALID;
    }

    if(m_will->y() >= m_enemy && m_will->y() + 15*(m_will->state() == SLIDING ? 1 : 0) <= m_enemy + ENEMY_SIZE){
        if(m_enemy_type == 0 || not m_is_punching){
            m_will->set_state(GAME_OVER);
        }
        else{
            enemy[enemy_it] = 0;
            m_enemy = INVALID;
            m_enemy_type = -1;
        }
    }
    printf("Saindo de do_collisions\n");
}

void TravelingWillPlayableLevel::check_game_over(){
    printf("Entrando em check_game_over\n");
    if(m_will->state() == GAME_OVER){
        m_y_speed = 0;
        m_x_speed = 0;
        m_next = m_current_level;
        remove_child(m_will);
        m_done = true;
    }
    printf("Saindo de check_game_over\n");
}

void TravelingWillPlayableLevel::update_platforms_position(){
    printf("Entrando em update_platforms_position\n");
    int height, current_x;
    for(int i = 0; i < 7; ++i){
        current_x = m_reverse_camera_x + 142*i;
        height = platform_height[i];

        if(current_x >= m_will->x() && current_x <= m_will->x() + WILL_WIDTH){
            m_floor = min(480.0 - height - WILL_HEIGHT, m_floor);
        }

        if(current_x + 142 >= m_will->x() && current_x <= m_will->x() + WILL_WIDTH){

            if(current_x + 56 + COLLECTABLE_DIMENSION >= m_will->x() && current_x + 56 <= m_will->x() + WILL_WIDTH){
                if(collectable[i]){
                    collectable_it = i;
                    m_collectable = 480.0 - collectable_height[i] - WILL_HEIGHT;
                    printf("%f %d\n", collectable_height[i], WILL_HEIGHT);
                }
                else{
                    m_collectable = INVALID;
                }
            }
            else{
                m_collectable = INVALID;
            }

            if(current_x + 48 + ENEMY_DIMENSION >= m_will->x() && current_x + 48 <= m_will->x() + WILL_WIDTH){
                if(enemy[i]){
                    enemy_it = i;
                    m_enemy = 480.0 - enemy_height[i] - WILL_HEIGHT;
                    m_enemy_type = enemy_type[i];
                    printf("%f %d\n", enemy_height[i], WILL_HEIGHT);
                }
                else{
                    m_enemy = INVALID;
                    m_enemy_type = -1;
                }
            }
            else{
                m_enemy = INVALID;
                m_enemy_type = -1;
            }
        }

        if(current_x >= m_will->x() && current_x <= m_will->x() + 30){
            m_floor = 480.0 - height - WILL_HEIGHT;
        }
    }
    printf("Saindo de update_platforms_position\n");
}

void TravelingWillPlayableLevel::update_counters(unsigned now){
    printf("Entrando em update_counters\n");
    //Update counters based on time
    sprite_counter += (now - m_start) * m_sprite_speed;
    m_camera_x += (now - m_start) * m_x_speed;
    m_reverse_camera_x -= (now - m_start) * m_x_speed;

    if(m_will->state() != GAME_OVER && not level_finished)
        m_audio_counter = now - m_audio_start;

    //Checking if music has ended
    if(m_audio_duration != -1 && m_audio_counter >= m_audio_duration){
        m_will->set_state(RUNNING);
        m_done = true;
    }

    //Reset value of reverse camera for each part of the level
    if(m_reverse_camera_x < -142 && m_current_level == "1"){
        m_reverse_camera_x += 142;
        platform_height.pop_front();
        collectable_height.pop_front();
        enemy_height.pop_front();
        collectable.pop_front();
        enemy.pop_front();
        enemy_type.pop_front();
    }

    //Reset background camera
    if(m_camera_x > 1704){
        m_camera_x -= 1704;
    }

    // Reset sprite counter
    if(sprite_counter > 5.9){
        sprite_counter -= 5.9;
    }
    printf("Saindo de update_counters\n");
}

void TravelingWillPlayableLevel::draw_self(Canvas *canvas, unsigned, unsigned){
    printf("Entrando em draw_self\n");
    canvas->clear();
    canvas->draw(m_background[0].get(), Rectangle(0, 0, 852, 480), 0, 0);

    canvas->draw(m_background[1].get(), Rectangle(m_camera_x/2, m_camera_y, 852, 480), 0, 0);
    canvas->draw(m_background[2].get(), Rectangle(m_camera_x, m_camera_y, 852, 480), 0, 0);

    //Draws each of the seven parts of the screen
    int height, current_x;
    for(int i = 0; i < 7; ++i){
        height = platform_height[i];
        current_x = m_reverse_camera_x+ 142*i;

        canvas->draw(m_level[height/50].get(), Rectangle(0, 0, 142, height), current_x, 480 - height);
        if(enemy[i]) canvas->draw(m_enemy_texture[enemy_type[i]].get(), Rectangle(ENEMY_DIMENSION * (int) sprite_counter, 0, 45, 45), current_x + 48, 480 - enemy_height[i]);
        if(collectable[i]) canvas->draw(m_collectable_texture.get(), Rectangle(COLLECTABLE_DIMENSION * (int) sprite_counter, 0, 30, 30), current_x + 56, 480 - collectable_height[i]);
    }

    double bar_width = 20 + (7.64 * 100 * m_audio_counter) / m_audio_duration;

    canvas->draw(m_progress_bar[0].get(), Rectangle(0, 0, 800, 19), 26, 18);
    canvas->draw(m_progress_bar[1].get(), Rectangle(0, 0, bar_width, 15), 30, 20);
    canvas->draw(m_progress_bar[2].get(), Rectangle(0, 0, 2, 15), 28, 20);
    canvas->draw(m_will_progress_bar.get(), Rectangle(0, 0, 20, 17), bar_width + 20, 20 - 1);

    canvas->draw(m_collectable_icon.get(), 705, 425);
    int x_digit = 805;
    int aux = n_collectables;
    do{
        canvas->draw(m_number.get(), Rectangle(23 * (aux % 10), 0, 23, 36), x_digit, 435);
        aux /= 10;
        x_digit -= 25;
    }while(aux);

    printf("Saindo de draw_self\n");
}