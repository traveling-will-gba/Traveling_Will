#include "traveling_will_level.h"
#include "traveling_will_playable_level.h"
#include "button.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/keyboard_event.h>

#include <unistd.h>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace ijengine;

static const int GAME_EVENT_JUMP =              1 << 4;
static const int GAME_EVENT_SLIDE_PRESSED =     1 << 5;
static const int GAME_EVENT_SLIDE_RELEASED =    1 << 6;
static const int GAME_EVENT_MENU_SELECT =       1 << 7;
static const int GAME_MOUSE_CLICK =             1 << 8;
static const int GAME_MOUSE_MOVEMENT =          1 << 9;
static const int GAME_MOUSE_MOTION =            1 << 10;
static const int GAME_EVENT_PUNCH =             1 << 11;
static const int NUMBER_OF_SCREENS =            12;
static const int WILL_HEIGHT =                  45;
static const int WILL_WIDTH =                   57;
static const int COLLECTABLE_DIMENSION =        34;
static const int COLLECTABLE_SIZE =             WILL_HEIGHT + COLLECTABLE_DIMENSION;
static const int ENEMY_DIMENSION =              45;
static const int ENEMY_SIZE =                   WILL_HEIGHT + ENEMY_DIMENSION;
static const int BACK_BUTTON =                  0;

TravelingWillPlayableLevel::TravelingWillPlayableLevel(int r, int g, int b, const string &current_level, const string& next_level, const string audio_path, 
int audio_duration) : 
	m_done(false), level_started(false), level_finished(false), m_will_enemy_type(-1), collectable_it(-10000000), enemy_it(-10000000), m_r(r), m_g(g), m_b(b),
	m_r(r), m_g(g), m_b(b), m_audio_duration(audio_duration), m_audio_start(0), m_audio_counter(0), m_start(-1), change(0), n_collectables(0), m_enemies(0),
	m_y_speed(0), sprite_counter(0), m_sprite_speed(1/170.0), m_camera_x(0), m_reverse_camera_x(1), m_reverse_camera_y(480), m_will_collectable(-100),
	m_will_enemy(-100), m_next(next_level), m_current_level(current_level), m_audio(audio_path), m_state(RUNNING), m_x_speed(5/19.0), m_camera_y(0),
	m_will_x(50), m_boss_x(690), m_boss_y(190), m_number(resources::get_texture("level/numbers.png")), 
	m_progress_bar[0](resources::get_texture("whole-progress-bar.png")), m_progress_bar[1](resources::get_texture("progress-bar.png")), 
	m_progress_bar[2](resources::get_texture("begin-progress-bar.png")){

	//Read level design from txt
	fstream level_design("res/" + m_current_level + "/level_design.txt");
	if(not level_design.is_open()){
		printf("Level design txt not available\n");
		exit(0);
	}
	level_design >> n_screens;

	platform_height = new double[n_screens + 1];
	collectable_height = new double[n_screens + 1];
	enemy_height = new double[n_screens + 1];

	collectable = new int[n_screens + 1];
	enemy = new int[n_screens + 1];
	enemy_type = new int[n_screens + 1];
	level_it = new int[n_screens + 1];

	for(int i = n_screens - 1; i >= 0; --i){
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
	}
	level_design.close();
	

    //Sets initial will height based on level design
	m_will_y = m_will_floor = 480 - platform_height[n_screens - 1] - WILL_HEIGHT;


	//Get platforms textures
	for(int i = 1; i < 9; i++){
	    m_level[i] = resources::get_texture(m_current_level + "/" + to_string(i*50) + ".png");
    }

	m_boss = resources::get_texture(m_current_level + "/perdeu.png");

	if(current_level == 1){
		m_background[0] = resources::get_texture(m_current_level + "/background_floresta_0.png");
		m_background[1] = resources::get_texture(m_current_level + "/background_floresta_1.png");
		m_background[2] = resources::get_texture(m_current_level + "/background_floresta_2.png");

		m_will[RUNNING] = resources::get_texture(m_current_level + "/will-running.png");
		m_will[JUMPING] = resources::get_texture(m_current_level + "/will-jumping.png");
		m_will[SLIDING] = resources::get_texture(m_current_level + "/will-sliding.png");
		m_will[FALLING] = resources::get_texture(m_current_level + "/will-falling.png");
		m_will[GAME_OVER] = resources::get_texture(m_current_level + "/will-gameover.png");
		m_will[PUNCHING]= resources::get_texture(m_current_level + "/will-punching.png");

		m_enemy[0] = resources::get_texture(m_current_level + "/enemy1.png");
		m_enemy[1] = resources::get_texture(m_current_level + "/enemy2.png");

		m_collectable = resources::get_texture(m_current_level + "/collectable.png");
		m_collectable_icon = resources::get_texture(m_current_level + "/collectable_icon.png");
	}

}
