#include "traveling_will_level.h"
#include "button.h"
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
static const int COLLECTABLE_DIMENSION =        30;
static const int COLLECTABLE_SIZE =             WILL_HEIGHT + COLLECTABLE_DIMENSION;
static const int ENEMY_DIMENSION =              45;
static const int ENEMY_SIZE =                   WILL_HEIGHT + ENEMY_DIMENSION;
static const int BACK_BUTTON =                  0;
static const int INVALID =                      -10000000;

TravelingWillLevel::TravelingWillLevel(int r, int g, int b, const string &current_level, const string& next_level, const string audio_path, int audio_duration)
    :  m_done(false), level_started(false), level_finished(false), m_will_enemy_type(-1), collectable_it(INVALID), enemy_it(INVALID), m_r(r), m_g(g), m_b(b),
    m_audio_duration(audio_duration), m_audio_start(0), m_audio_counter(0), m_start(-1), change(0), n_collectables(0), n_enemies(0), start_cutscene_counter(1),
    final_cutscene_counter(1), m_y_speed(0), sprite_counter(0), m_sprite_speed(0), m_camera_x(0), m_reverse_camera_x(1), m_reverse_camera_y(480),
    m_will_collectable(-100), m_will_enemy(-100), m_next(next_level), m_current_level(current_level), m_audio(audio_path), is_selected(false) {

        printf("current_level: [%s]\n", m_current_level.c_str());
        printf("Audio of level [%s]\n", m_audio.c_str());

        m_buttons.clear();

        if(m_current_level == "menu"){
            m_camera_y = 0;

            m_background[0] = resources::get_texture(m_current_level + "/menu-tela.png");
            m_buttons.push_back(new Button(0, m_current_level, 20, 400, "voltar-botao.png", 144, 40, 0));
            m_buttons.push_back(new Button(1, m_current_level, 260, 280, "opcoes-botao.png", 270, 70, 1));
            m_buttons.push_back(new Button(2, m_current_level, 260, 360, "sair-botao.png", 270, 70, 1));
            m_buttons.push_back(new Button(3, m_current_level, 700, 400, "creditos-botao.png", 144, 40, 1));
            m_buttons.push_back(new Button(4, m_current_level, 260, 200, "novo-jogo-botao.png", 270, 70, 1));

            m_buttons.push_back(new Button(5, m_current_level + "/opcoes", 260, 200, "volume-botao.png", 144, 40, 0));
            m_buttons.push_back(new Button(6, m_current_level + "/opcoes", 260, 280, "tela-cheia-botao.png", 144, 40, 0));
            m_buttons.push_back(new Button(7, m_current_level + "/fases", 80, 210, "fase-1.png", 200, 150, 0));
            m_buttons.push_back(new Button(8, m_current_level + "/fases", 320, 210, "fase-2.png", 200, 150, 0));
            m_buttons.push_back(new Button(9, m_current_level + "/fases", 560, 210, "fase-3.png", 200, 150, 0));

            m_start = -1;
        }
        else if(m_current_level == "cutscene-intro"){
            m_cutscene_speed = 1/300.0;

            for(int i=1; i<=9; i++){
                m_start_cutscene[i] = resources::get_texture(m_current_level + "/start_cutscene-" + to_string(i) + ".png");
            }

            m_buttons.push_back(new Button(0, m_current_level, 700, 410, "pular-botao.png", 142, 50, 1));
        }
        else if(m_current_level == "1"){
            //Sets level information
            m_x_speed = 5/19.0;
            m_sprite_speed = 1/170.0;
            m_camera_y = 0;
            change = 0;
            m_boss_x = 690;
            m_boss_y = 190;
            m_background[0] = resources::get_texture(m_current_level + "/background_floresta_0.png");
            m_background[1] = resources::get_texture(m_current_level + "/background_floresta_1.png");
            m_background[2] = resources::get_texture(m_current_level + "/background_floresta_2.png");


            m_enemy[0] = resources::get_texture(m_current_level + "/enemy1.png");
            m_enemy[1] = resources::get_texture(m_current_level + "/enemy2.png");

            m_collectable = resources::get_texture(m_current_level + "/collectable.png");
            m_collectable_icon = resources::get_texture(m_current_level + "/collectable_icon.png");

            m_number = resources::get_texture(m_current_level + "/numbers.png");

            m_progress_bar[0] = resources::get_texture("whole-progress-bar.png");
            m_progress_bar[1] = resources::get_texture("progress-bar.png");
            m_progress_bar[2] = resources::get_texture("begin-progress-bar.png");
            m_will_progress_bar = resources::get_texture("tiny-will-progress-bar.png");

            //Read level design from txt
            fstream level_design("res/" + m_current_level + "/level_design.txt");
            printf("[%s]\n", ("res/" + m_current_level + "/level_design.txt").c_str());
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
                printf("platform_height[%d] = %.2f\n", i, platform_height[i]);
            }

            level_design.close();

            m_floor = 480 - platform_height[n_screens - 1] - WILL_HEIGHT;

            //Get platforms textures
            for(int i = 1; i < 9; ++i){
                m_level[i] = resources::get_texture(m_current_level + "/" + to_string(i*50) + ".png");
            }

            m_boss = resources::get_texture(m_current_level + "/perdeu.png");

            m_will = new Will(50, 480 - platform_height[n_screens - 1] - WILL_HEIGHT);
            add_child(m_will);

            m_start = -1;
        }
        else if(m_current_level == "cutscene-end"){
            m_cutscene_speed = 1/300.0;

            for(int i=1; i<=3; i++){
                m_end_cutscene[i] = resources::get_texture(m_current_level + "/end_cutscene-" + to_string(i) + ".png");
            }
        }

        for(auto btn : m_buttons){
            add_child(btn);
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
    if(m_current_level == "menu"){
        if(event.id() == GAME_EVENT_SLIDE_PRESSED){
            return true;
        }

        if(event.id() == GAME_MOUSE_CLICK){
            double mouse_x = event.get_property<double>("x");
            double mouse_y = event.get_property<double>("y");

            for(auto btn : m_buttons){
                if(btn->able_to_draw() == 0) continue;

                int min_x = btn->x(), max_x = min_x + btn->w();
                int min_y = btn->y(), max_y = min_y + btn->h();

                if(mouse_x >= min_x && mouse_x <= max_x && mouse_y >= min_y && mouse_y <= max_y){
                    switch(btn->id()){
                        case BACK_BUTTON:
                            m_background[0] = resources::get_texture(m_current_level + "/menu-tela.png");
                            for(auto button : m_buttons){
                                if(button->level() == "menu" && button->id() != BACK_BUTTON)
                                    button->set_able_to_draw(1);
                                else
                                    button->set_able_to_draw(0);
                            }
                            break;
                        case 1:
                            m_background[0] = resources::get_texture(m_current_level + "/opcoes-tela.png");

                            for(auto button : m_buttons){
                                if(button->level() == "menu")
                                    button->set_able_to_draw(0);
                                else if(button->level() == m_current_level + "/opcoes")
                                    button->set_able_to_draw(1);

                                if(button->id() == BACK_BUTTON && button->able_to_draw() == 0)
                                    button->set_able_to_draw(1);
                            }
                            break;
                        case 2:
                            exit(1);
                            break;
                        case 3:
                            m_background[0] = resources::get_texture(m_current_level + "/creditos-tela.png");
                            for(auto button : m_buttons){
                                if(button->level() == "menu")
                                    button->set_able_to_draw(0);
                                else if(button->level() == m_current_level + "/creditos")
                                    button->set_able_to_draw(1);

                                if(button->id() == BACK_BUTTON && button->able_to_draw() == 0)
                                    button->set_able_to_draw(1); 
                            }
                            break;
                        case 4:
                            m_background[0] = resources::get_texture(m_current_level + "/fases-tela.png");

                            for(auto button : m_buttons){
                                if(button->level() == "menu")
                                    button->set_able_to_draw(0);
                                else if(button->level() == m_current_level + "/fases")
                                    button->set_able_to_draw(1);

                                if(button->id() == BACK_BUTTON && button->able_to_draw() == 0)
                                    button->set_able_to_draw(1);
                            }
                            break;
                        case 5:
                            printf("VOLUME\n");
                            break;
                        case 6:
                            printf("TELA CHEIA\n");
                            break;
                        case 7:
                            // FIXME
                            is_selected = true;
                            break;
                        case 8:
                            // FIXME
                            is_selected = true;
                            printf("FASE 2\n");
                            break;
                        case 9:
                            // FIXME
                            is_selected = true;
                            printf("FASE 3\n");
                            break;
                        default:
                            break;
                    }

                    return true;
                }

            }

        }
    }

    else if(m_current_level == "cutscene-intro"){
        if(event.id() == GAME_MOUSE_CLICK){
            double mouse_x = event.get_property<double>("x");
            double mouse_y = event.get_property<double>("y");

            for(auto btn : m_buttons){
                if(btn->able_to_draw() == 0) continue;

                int min_x = btn->x(), max_x = min_x + btn->w();
                int min_y = btn->y(), max_y = min_y + btn->h();

                if(mouse_x >= min_x && mouse_x <= max_x && mouse_y >= min_y && mouse_y <= max_y){
                    switch(btn->id()){
                        case 0:
                            m_done = true;
                            break;
                        default:
                            break;
                    }

                    return true;
                }

            }

        }
    }

    return false;
}

void TravelingWillLevel::update_self(unsigned now, unsigned){
    if(m_current_level == "menu"){
        if(is_selected){
            m_done = true;
        }

        return;
    }

    if(m_current_level == "cutscene-intro"){
        if((now - m_start) >= 28000){
            m_done = true;
        }

        return;
    }

    if(m_current_level == "cutscene-end"){
        if((now - m_start) >= 9000){
            m_done = true;
        }

        return;
    }

    if(m_start == -1){
        m_start = now;
        m_audio_start = m_start;
    }

    final_cutscene_counter += (now - m_start) * m_cutscene_speed;

    //Update counters based on time
    sprite_counter += (now - m_start) * m_sprite_speed;
    m_camera_x += (now - m_start) * m_x_speed;
    m_reverse_camera_x += (now - m_start) * m_x_speed;

    if(m_will->state() != GAME_OVER && not level_finished)
        m_audio_counter = now - m_audio_start;

    //Checking if music has ended
    if(m_audio_duration != -1 && m_audio_counter >= m_audio_duration){
        m_will->set_state(RUNNING);
        m_done = true;
    }

    //Reset value of reverse camera for each part of the level
    if(m_reverse_camera_x > 142 && m_current_level == "1"){
        m_reverse_camera_x -= 142;
        ++change;
    }

    //Reset background camera
    if(m_camera_x > 1704){
        m_camera_x -= 1704;
    }

    // Reset sprite counter
    if(sprite_counter > 5.9){
        sprite_counter -= 5.9;
    }

    do_collisions(now);

    update_platforms_position();

    check_game_over();

    m_start = now;
}

void TravelingWillLevel::draw_self(Canvas *canvas, unsigned now, unsigned){
    if(m_current_level == "menu"){
        canvas->clear();
        canvas->draw(m_background[0].get(), Rectangle(0, 0, 852, 480), 0, 0);
    }

    else if(m_current_level == "cutscene-intro"){
        canvas->clear();
        canvas->draw(m_start_cutscene[1 + (now - m_start) / 3200].get(), Rectangle(0, 0, 852, 480), 0, 0);
    }

    else if(m_current_level == "cutscene-end"){
        canvas->clear();
        canvas->draw(m_end_cutscene[1 + (now - m_start) / 3200].get(), Rectangle(0, 0, 852, 480), 0, 0);
    }

    else if(m_current_level == "1"){
        canvas->clear();
        canvas->draw(m_background[0].get(), Rectangle(0, 0, 852, 480), 0, 0);

        canvas->draw(m_background[1].get(), Rectangle(m_camera_x/2, m_camera_y, 852, 480), 0, 0);
        canvas->draw(m_background[2].get(), Rectangle(m_camera_x, m_camera_y, 852, 480), 0, 0);

        //Draws each of the seven parts of the screen
        int aux = 0, it, height;
        for(int i =(int)m_reverse_camera_x; i <= 994; i += 142){
            it = level_it[aux++];
            height = platform_height[it];
            canvas->draw(m_level[height/50].get(), Rectangle(0, 0, 142, height), 852 - i, 480 - height);
            if(enemy[it]) canvas->draw(m_enemy[enemy_type[it]].get(), Rectangle(ENEMY_DIMENSION * (int) sprite_counter, 0, 45, 45), 852 - i + 48, 480 - enemy_height[it]);
            if(collectable[it]) canvas->draw(m_collectable.get(), Rectangle(COLLECTABLE_DIMENSION * (int) sprite_counter, 0, 30, 30), 852 - i + 56, 480 - collectable_height[it]);
        }

        double bar_width = 20 + (7.64 * 100 * m_audio_counter) / m_audio_duration;

        canvas->draw(m_progress_bar[0].get(), Rectangle(0, 0, 800, 19), 26, 18);
        canvas->draw(m_progress_bar[1].get(), Rectangle(0, 0, bar_width, 15), 30, 20);
        canvas->draw(m_progress_bar[2].get(), Rectangle(0, 0, 2, 15), 28, 20);
        canvas->draw(m_will_progress_bar.get(), Rectangle(0, 0, 20, 17), bar_width + 20, 20 - 1);

        canvas->draw(m_collectable_icon.get(), 705, 425);

        aux = n_collectables;
        int x_digit = 805;
        do{
            canvas->draw(m_number.get(), Rectangle(23 * (aux % 10), 0, 23, 36), x_digit, 435);
            aux /= 10;
            x_digit -= 25;
        }while(aux);

        if(m_state == GAME_OVER){
            canvas->draw(m_boss.get(), 100, 100);
        }
    }
}

void TravelingWillLevel::do_collisions(unsigned now){
    //Test Will colision
    if((int) m_will->y() > (int) m_floor){
        m_will->set_state(GAME_OVER);
    }

    //Start jump if Will is at the end of a cliff
    if(m_will->y() < m_floor && m_will->state() != JUMPING && m_will->state() != FALLING && m_will->state() != GAME_OVER){
        m_will->set_y_speed(1/300.0 * 0.5);
        m_will->set_state(FALLING);
    }

    //Calculate jump speed and stop jump if hits the ground
    if(m_will->state() == JUMPING || m_will->state() == FALLING){
        m_will->set_y_speed(m_will->speed() + ((now - m_start)/300.0 * 0.5));
        if(m_will->speed() >= 0.001) m_will->set_state(FALLING);

        if(m_will->y() + (now - m_start) * m_will->speed() > m_floor){
            m_will->set_y(m_floor);
            m_will->set_y_speed(0);
            m_will->set_state(RUNNING);
        }
    }

    if(m_will->y() >= m_will_collectable && m_will->y() + 15*(m_will->state() == SLIDING ? 1 : 0) <= m_will_collectable + COLLECTABLE_SIZE){
        ++n_collectables;

        collectable[collectable_it] = 0;
        m_will_collectable = INVALID;
    }

    if(m_will->y() >= m_will_enemy && m_will->y() + 15*(m_will->state() == SLIDING ? 1 : 0) <= m_will_enemy + ENEMY_SIZE){
        if(m_will_enemy_type == 0 || not m_is_punching){
            m_will->set_state(GAME_OVER);
        }else{
            enemy[enemy_it] = 0;
            m_will_enemy = INVALID;
            m_will_enemy_type = -1;
        }
    }

}

void TravelingWillLevel::check_game_over(){
    if(m_will->state() == GAME_OVER){
        m_y_speed = 0;
        m_x_speed = 0;
        m_next = m_current_level;
        remove_child(m_will);
        m_done = true;
    }
}

void TravelingWillLevel::update_platforms_position(){
    int aux = n_screens - 7, height, it;

    int aux_it = 0;
    for(int i =(int)m_reverse_camera_x; i <= 994; i += 142){
        it = aux - change;
        level_it[aux_it++] = max(it, 0);

        if(it >= 0){
            height = platform_height[it];
            if(852 - i >= m_will->x() && 852 - i <= m_will->x() + WILL_WIDTH){
                m_floor = min(480.0 - height - WILL_HEIGHT, m_floor);
            }

            if(852 - i + 142 >= m_will->x() && 852 - i <= m_will->x() + WILL_WIDTH){

                if(852 - i + 56 + COLLECTABLE_DIMENSION >= m_will->x() && 852 - i + 56 <= m_will->x() + WILL_WIDTH){
                    if(collectable[it]){
                        collectable_it = it;
                        m_will_collectable = 480.0 - collectable_height[it] - WILL_HEIGHT;
                        printf("%f %d\n", collectable_height[it], WILL_HEIGHT);
                    }else{
                        m_will_collectable = INVALID;
                    }
                }else{
                    m_will_collectable = INVALID;
                }

                if(852 - i + 48 + ENEMY_DIMENSION >= m_will->x() && 852 - i + 48 <= m_will->x() + WILL_WIDTH){
                    if(enemy[it]){
                        enemy_it = it;
                        m_will_enemy = 480.0 - enemy_height[it] - WILL_HEIGHT;
                        m_will_enemy_type = enemy_type[it];
                        printf("%f %d\n", enemy_height[it], WILL_HEIGHT);
                    }else{
                        m_will_enemy = INVALID;
                        m_will_enemy_type = -1;
                    }
                }else{
                    m_will_enemy = INVALID;
                    m_will_enemy_type = -1;
                }
            }

            if(852 - i >= m_will->x() && 852 - i <= m_will->x() + 30){
                m_floor = 480.0 - height - WILL_HEIGHT;
            }
        }
        aux++;
    }
}