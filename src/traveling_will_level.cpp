#include "traveling_will_level.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/keyboard_event.h>

#include <unistd.h>

#include <iostream>
#include <fstream>

using namespace std;
using namespace ijengine;

static const int GAME_EVENT_JUMP =              1 << 4;
static const int GAME_EVENT_SLIDE_PRESSED =     1 << 5;
static const int GAME_EVENT_SLIDE_RELEASED =    1 << 6;
static const int GAME_EVENT_MENU_SELECT =       1 << 7;
static const int NUMBER_OF_SCREENS =            12;
static const int WILL_HEIGHT =                  45;
static const int WILL_WIDTH =                   57;
static const int COLLECTABLE_SIZE =             WILL_HEIGHT + 30;

TravelingWillLevel::TravelingWillLevel(int r, int g, int b, const string &current_level, const string& next_level, const string audio_path)
    : m_r(r), m_g(g), m_b(b), m_done(false), m_next(next_level), m_sprite_speed(0), m_start(-1), m_camera_x(0), m_reverse_camera_x(1), m_reverse_camera_y(480), m_y_speed(0), m_will_collectable(-100), n_collectables(0), turn_off_collectable(false), change(0), m_current_level(current_level), m_audio(audio_path), m_state(NOTHING), sprite_counter(0), level_started(false), level_finished(false), start_cutscene_counter(1), final_cutscene_counter(1), m_cutscene_speed(1/300.0) {

        printf("current_level: [%s]\n", m_current_level.c_str());
        printf("Audio of level [%s]\n", m_audio.c_str());

        if(m_current_level == "menu"){
            m_camera_y = 0;
            printf("[%s]\n", (m_current_level + "/Menu.png").c_str());
            m_background[0] = resources::get_texture(m_current_level + "/Menu.png");
        }
        else if(m_current_level == "1"){
            //Sets level information
            m_state = RUNNING;
            m_camera_y = 0;
            change = 0;
            m_will_x = 50;
            m_boss_x = 690;
            m_boss_y = 190;
            m_background[0] = resources::get_texture(m_current_level + "/background_floresta_0.png");
            m_background[1] = resources::get_texture(m_current_level + "/background_floresta_1.png");
            m_background[2] = resources::get_texture(m_current_level + "/background_floresta_2.png");

            m_will[RUNNING] = m_will[SELECTING] = m_will[NOTHING] = resources::get_texture(m_current_level + "/will-running.png");
            m_will[JUMPING] = resources::get_texture(m_current_level + "/will-jumping.png");
            m_will[SLIDING] = resources::get_texture(m_current_level + "/will-sliding.png");
            m_will[FALLING] = resources::get_texture(m_current_level + "/will-falling.png");
            m_will[GAME_OVER] = resources::get_texture(m_current_level + "/will-gameover.png");

			for(int i=1; i<=4; i++){
				m_start_cutscene[i] = resources::get_texture(m_current_level + "/start_cutscene-" + to_string(i) + ".png");
			}

			for(int i=1; i<=4; i++){
				m_final_cutscene[i] = resources::get_texture(m_current_level + "/final_cutscene-" + to_string(i) + ".png");
			}

            m_enemy[0] = resources::get_texture(m_current_level + "/enemy1.png");
            m_enemy[1] = resources::get_texture(m_current_level + "/enemy2.png");

            m_collectable = resources::get_texture(m_current_level + "/collectable.png");

            //Read level design from txt
            fstream level_design("res/" + m_current_level + "/level_design.txt");
            printf("[%s]\n", ("res/" + m_current_level + "/level_design.txt").c_str());
            if(not level_design.is_open()){
                printf("Deu ruim\n");
                //exit(0);
            }
            level_design >> n_screens;
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
                printf("platform_height[%d] = %d\n", i, platform_height[i]);
           	}
            level_design.close();

            //Sets initial will height based on level design
            m_will_y = m_will_floor = 480 - platform_height[n_screens - 1] - WILL_HEIGHT;
            printf("%.2f %.2f\n", m_will_y, m_will_floor);

            //Get platforms textures
            for(int i = 1; i < 9; ++i){
                printf("Deu bom em %d [%s]\n", i,(m_current_level  + "/" + to_string(i*50) + ".png").c_str());
                m_level[i] = resources::get_texture(m_current_level + "/" + to_string(i*50) + ".png");
            }

            m_boss = resources::get_texture(m_current_level + "/perdeu.png");
            
            //Set camera and sprite speeds
            m_x_speed = 0;
            m_sprite_speed = 0;
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
        m_state = SLIDING;
        return true;
    }

    if(event.id() == GAME_EVENT_SLIDE_RELEASED && m_state == SLIDING){
        m_state = RUNNING;
        return true;
    }

    return true;
}

void TravelingWillLevel::update_self(unsigned now, unsigned){
    if(m_start == -1)
        m_start = now;

	if(not level_started){
		start_cutscene_counter += m_cutscene_speed;

		if(start_cutscene_counter >= 4.9){
			level_started = true;

			//Set camera and sprite speeds
            m_x_speed = 4/19.0;
            m_sprite_speed = 1/170.0;
		}
	}

	if(level_finished){
		m_x_speed = 0;
		m_sprite_speed = 0;

		final_cutscene_counter += m_cutscene_speed;

		if(final_cutscene_counter >= 4.9){
			exit(0);
		}
	}

    //printf("ms: %d ",now);

    /*
    if(m_state == NOTHING) printf("NOTHING\n");
    if(m_state == RUNNING) printf("RUNNING\n");
    if(m_state == JUMPING) printf("JUMPING\n");
    if(m_state == SELECTING) printf("SELECTING\n");
    if(m_state == SLIDING) printf("SLIDING\n");
	*/

    if(m_state == SELECTING){
        m_state = RUNNING;
        m_done = true;
    }

    //Reset value of reverse camera for each part of the level
    if(m_reverse_camera_x > 142 && m_current_level == "1"){
    	//printf("m_r_c = %.2f\n", m_reverse_camera_x);
    	m_reverse_camera_x -= 142;
    	//printf("change = %d\n", change);
    	++change;
    }

    //Reset background camera
    if(m_camera_x > 852){
        m_camera_x -= 852;
    }

    //Reset sprite counter
    if(sprite_counter > 5.9){
        sprite_counter -= 5.9;
    }

    //Test Will colision
    if(m_will_y + (now - m_start) * m_y_speed > m_will_floor + 20){
        m_state = GAME_OVER;
    }

    printf("m_will_y = %f x %f (%d) (%d)\n", m_will_y, m_will_collectable, n_collectables, turn_off_collectable);
    //printf("TEMPO %d (%d): %.2f >= %.2f && %.2f <= %.2f\n", now, n_collectables, m_will_y + (now - m_start) * m_y_speed, m_will_collectable, m_will_y + (now - m_start) * m_y_speed, m_will_collectable + COLLECTABLE_SIZE);
    if(m_current_level != "menu" && m_will_y + (now - m_start) * m_y_speed >= m_will_collectable && m_will_y + (now - m_start) * m_y_speed <= m_will_collectable + COLLECTABLE_SIZE){
        printf("AQUIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIIII\n");
        ++n_collectables;
        printf("EITAAAAA: %.2f >= %.2f && %.2f <= %.2f\n", m_will_y + (now - m_start) * m_y_speed, m_will_collectable, m_will_y + (now - m_start) * m_y_speed, m_will_collectable + COLLECTABLE_SIZE);
        
        //if(n_collectables == 2) exit(0);
        turn_off_collectable = true;
        m_will_collectable = -10000000;
    }

    //Start jump if Will is at the end of a cliff
    if(m_will_y < m_will_floor && m_state != JUMPING && m_state != FALLING){
        //printf("Tá de boa\n");
        m_y_speed = 1/300.0 * 0.5;
        m_state = FALLING;
    }

    //Calculate jump speed and stop jump if hits the ground
    if(m_state == JUMPING || m_state == FALLING){
        m_y_speed += (now - m_start)/300.0 * 0.5;
        if(m_y_speed >= 0.001) m_state = FALLING;

        if(m_will_y + (now - m_start) * m_y_speed > m_will_floor){
            m_will_y = m_will_floor;
            m_y_speed = 0;
            m_state = RUNNING;
        }
    }

    //Update counters based on time
    sprite_counter += (now - m_start) * m_sprite_speed;
    m_camera_x += (now - m_start) * m_x_speed;
    m_reverse_camera_x += (now - m_start) * m_x_speed;
    m_will_y += (now - m_start) * m_y_speed;

    m_start = now;
}

void TravelingWillLevel::draw_self(Canvas *canvas, unsigned, unsigned){
    canvas->clear();
    canvas->draw(m_background[0].get(), Rectangle(0, 0, 852, 480), 0, 0);

    if(m_current_level != "menu"){
		if(not level_started){
			canvas->draw(m_start_cutscene[(int)start_cutscene_counter].get(), Rectangle(0, 0, 852, 480), 0, 0);
			return;
		}

		if(level_finished){
			canvas->draw(m_final_cutscene[(int)final_cutscene_counter].get(), Rectangle(0, 0, 852, 480), 0, 0);
			return;
		}

        canvas->draw(m_background[1].get(), Rectangle(m_camera_x/2, m_camera_y, 852, 480), 0, 0);
        canvas->draw(m_background[2].get(), Rectangle(m_camera_x, m_camera_y, 852, 480), 0, 0);

        //printf("----------\n");
        int aux = n_screens - 7, height, it;
        bool finished = true;

        //Draws each of the seven parts of the screen
        for(int i =(int)m_reverse_camera_x; i <= 994; i += 142){
            //printf("%d, %d, %d\n", 852 - i, aux - change, change);
        	it = aux - change;
            if(it >= 0){
                finished = false;
                height = platform_height[it];
                if(852 - i >= m_will_x && 852 - i <= m_will_x + WILL_WIDTH){
                    //printf("%.2f x %.2f\n",480.0 - height - WILL_HEIGHT, m_will_floor);
                    m_will_floor = min(480.0 - height - WILL_HEIGHT, m_will_floor);
                    //printf("Entrou %.2f\n", m_will_y);
                    if(turn_off_collectable){
                        collectable[it] = 0;
                        turn_off_collectable = false;
                        //printf("Coletou\n");
                    }
                    if(852 - i + 86 >= m_will_x && 852 - i + 56 <= m_will_x + WILL_WIDTH){
                        if(collectable[it]){
                            m_will_collectable = 480.0 - collectable_height[it] - WILL_HEIGHT;
                            printf("%f %f\n", collectable_height[it], WILL_HEIGHT);
                            //exit(0);   
                        }else{
                            m_will_collectable = -1000000000;
                        }   
                    }else{
                        m_will_collectable = -1000000000;
                    }

                }
                if(852 - i >= m_will_x && 852 - i <= m_will_x + 30){
                    m_will_floor = 480.0 - height - WILL_HEIGHT;
                }

				
					

				//printf("m_will_collectable = %.2f, m_will_x = %.2f\n", m_will_collectable, m_will_x);


				//printf("height = %d\n", height);
				canvas->draw(m_level[height/50].get(), Rectangle(0, 0, 142, height), 852 - i, 480 - height);
				if(enemy[it]) canvas->draw(m_enemy[enemy_type[it]].get(), Rectangle(45 * (int) sprite_counter, 0, 45, 45), 852 - i, 480 - enemy_height[it]);
				if(collectable[it]) canvas->draw(m_collectable.get(), Rectangle(30 * (int) sprite_counter, 0, 30, 30), 852 - i + 56, 480 - collectable_height[it]);
				//printf("v = %d\n", height);
			}
			aux++;
		}
		//printf("%.2f e %.2f\n", m_will_y, m_will_floor);

		//Sets GAME OVER if it ran out of platforms
        if(finished) m_state = GAME_OVER;
        //for(int i = change; i < 8 + change; ++i){
        //	printf("Desenhou %d em %.2f,%.2f\n", i, m_reverse_camera_x - 142*(8 - i), m_will_y);
        //	canvas->draw(m_level[i].get(), m_reverse_camera_x - 142*(12 - i), m_will_y);
        //}

        //printf("will height = %.2f\n", m_will_y);
        //Draw Will based on its position
        //printf("sc = %.2f e dist = %.3f\n", sprite_counter, WILL_WIDTH* (int)sprite_counter);
        //printf("Drawing Will with state=%d, width=%d, ,sprite_counter=%f, x=%f, y=%f\n", m_state, WILL_WIDTH* (int) sprite_counter, sprite_counter, m_will_x, m_will_y + 15*(m_state == SLIDING ? 1 : 0));
        canvas->draw(m_will[m_state].get(), Rectangle(WILL_WIDTH* (int) sprite_counter, 0, WILL_WIDTH, WILL_HEIGHT - 15*(m_state == SLIDING ? 1 : 0)), m_will_x, m_will_y + 15*(m_state == SLIDING ? 1 : 0));
        //canvas->draw(m_boss.get(), m_boss_x - m_camera_x*2, m_boss_y);

        //Stop cameras and shows message if it's GAME OVER
        if(m_state == GAME_OVER){
            m_y_speed = 0;
            m_x_speed = 0;
            canvas->draw(m_boss.get(), 100, 100);
        }
    }
}
