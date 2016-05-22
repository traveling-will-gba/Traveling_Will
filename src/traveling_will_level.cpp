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

TravelingWillLevel::TravelingWillLevel(int r, int g, int b, const string &current_level, const string& next_level, const string audio_path)
    : m_r(r), m_g(g), m_b(b), m_done(false), m_next(next_level), m_sprite_speed(0), m_start(-1), m_camera_x(0), m_reverse_camera_x(1), m_reverse_camera_y(480),
    m_y_speed(0), change(0), m_current_level(current_level), m_audio(audio_path), m_state(NOTHING), sprite_counter(0) {

        printf("current_level: [%s]\n", m_current_level.c_str());
        printf("Audio of level [%s]\n", m_audio.c_str());

        if(m_current_level == "menu"){
            m_camera_y = 0;
            printf("[%s]\n", (m_current_level + "/Menu.png").c_str());
            m_background[0] = resources::get_texture(m_current_level + "/Menu.png");
        }
        else if(m_current_level == "1"){
            m_state = RUNNING;
            m_camera_y = 0;
            change = 0;
            m_will_x = 50;
            m_boss_x = 690;
            m_boss_y = 190;
            m_background[0] = resources::get_texture(m_current_level + "/background_floresta_0.png");
            m_background[1] = resources::get_texture(m_current_level + "/background_floresta_1.png");
            m_background[2] = resources::get_texture(m_current_level + "/background_floresta_2.png");

            m_will[RUNNING] = m_will[SELECTING] = m_will[NOTHING] = m_will[GAME_OVER] = resources::get_texture(m_current_level + "/will.png");
            m_will[JUMPING] = resources::get_texture(m_current_level + "/will-jump.png");
            m_will[SLIDING] = resources::get_texture(m_current_level + "/will-slide.png");


            current_image = 0;
            int aux2[] = {300, 200, 150, 400, 150, 200, 400, 300, 200, 150, 400, 400, 400};
            int platform_height;
            fstream level_design("res/" + m_current_level + "/level_design.txt");
            printf("[%s]\n", ("res/" + m_current_level + "/level_design.txt").c_str());
            if(not level_design.is_open()){
                printf("Deu ruim\n");
                exit(0);
            }
            level_design >> n_screens;
            for(int i = n_screens - 1; i >= 0; --i){
                level_design >> level_image_height[i];
                printf("level_image_height[%d] = %d\n", i, level_image_height[i]);
           	}
            level_design.close();


            m_will_y = m_will_floor = 480 - level_image_height[n_screens - 1] - WILL_HEIGHT;
            printf("%.2f %.2f\n", m_will_y, m_will_floor);

            for(int i = 1; i < 9; ++i){
                printf("Deu bom em %d [%s]\n", i,(m_current_level  + "/" + to_string(i*50) + ".png").c_str());
                m_level[i] = resources::get_texture(m_current_level + "/" + to_string(i*50) + ".png");
            }

            m_boss = resources::get_texture(m_current_level + "/perdeu.png");
            m_x_speed = 4/19.0;
            m_sprite_speed = 1/170.0;
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

    //printf("m_r_cam = %.2f, m_camera_x = %.2f\n", m_reverse_camera_x, m_camera_x);
    if(m_reverse_camera_x > 142 && m_current_level == "1"){
    	//printf("m_r_c = %.2f\n", m_reverse_camera_x);
    	m_reverse_camera_x -= 142;
    	//printf("change = %d\n", change);
    	++change;
    }

    if(m_camera_x > 852){
        m_camera_x -= 852;
    }

    if(sprite_counter > 5.9){
        sprite_counter -= 5.9;
    }

    //printf("%.2f x %.2f\n", m_will_y, m_will_floor);
    if(m_will_y + (now - m_start) * m_y_speed > m_will_floor + 20){
        m_state = GAME_OVER;
    }

    if(m_will_y < m_will_floor && m_state != JUMPING){
        //printf("Tá de boa\n");
        m_y_speed = 1/300.0 * 0.5;
        m_state = JUMPING;
    }


    if(m_state == JUMPING){
        m_y_speed += (now - m_start)/300.0 * 0.5;

        if(m_will_y + (now - m_start) * m_y_speed > m_will_floor){
            m_will_y = m_will_floor;
            m_boss_y = 190;
            m_y_speed = 0;
            m_state = RUNNING;
        }
    }

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
        canvas->draw(m_background[1].get(), Rectangle(m_camera_x/2, m_camera_y, 852, 480), 0, 0);
        canvas->draw(m_background[2].get(), Rectangle(m_camera_x, m_camera_y, 852, 480), 0, 0);

        printf("----------\n");
        int aux = n_screens - 7, height;
        bool finished = true;
        for(int i =(int)m_reverse_camera_x; i <= 994; i += 142){
        	//printf("%d, %d, %d\n", 852 - i, aux - change, change);
        	if(aux - change >= 0){
                finished = false;
                height = level_image_height[aux - change];
                if(852 - i >= m_will_x && 852 - i <= m_will_x + WILL_WIDTH){
                    printf("%.2f x %.2f\n",480.0 - height - WILL_HEIGHT, m_will_floor);
                    m_will_floor = min(480.0 - height - WILL_HEIGHT, m_will_floor);
                    printf("Entrou %.2f\n", m_will_y);
                }
                if(852 - i == m_will_x){
                    m_will_floor = 480.0 - height - WILL_HEIGHT;
                }
                //printf("height = %d\n", height);
        		canvas->draw(m_level[height/50].get(), Rectangle(0, 0, 142, height), 852 - i, 480 - height);
                //printf("v = %d\n", height);
            }
        	aux++;
        }
        printf("%.2f e %.2f\n", m_will_y, m_will_floor);
        
        if(finished) m_state = GAME_OVER;
        //for(int i = change; i < 8 + change; ++i){
        //	printf("Desenhou %d em %.2f,%.2f\n", i, m_reverse_camera_x - 142*(8 - i), m_will_y);
        //	canvas->draw(m_level[i].get(), m_reverse_camera_x - 142*(12 - i), m_will_y);
        //}

        //printf("will height = %.2f\n", m_will_y);
        printf("sc = %.2f e dist = %.3f\n", sprite_counter, WILL_WIDTH* (int)sprite_counter);
        canvas->draw(m_will[m_state].get(), Rectangle(WILL_WIDTH* (int) sprite_counter, 0, WILL_WIDTH, WILL_HEIGHT - 15*(m_state == SLIDING ? 1 : 0)), m_will_x, m_will_y + 15*(m_state == SLIDING ? 1 : 0));
        //canvas->draw(m_boss.get(), m_boss_x - m_camera_x*2, m_boss_y);

        if(m_state == GAME_OVER){
            m_y_speed = 0;
            m_x_speed = 0;
            canvas->draw(m_boss.get(), 100, 100);
        }
    }
}
