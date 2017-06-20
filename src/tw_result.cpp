#include "tw_result.h"

#include "../engine/include/canvas.h"
#include "../engine/include/engine.h"
#include "../engine/include/keyboard_event.h"

#include <unistd.h>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace ijengine;

TWResult::TWResult(const string &current_level, const string& next_level, const string, int){
    m_current_level = current_level;
    m_next = next_level;
    m_done = false;
    m_win = false;
    m_state = NOTHING;
    m_start = -1;
    m_audio = "";

    m_sprite_speed = 1/85.0;
    m_sprite_counter = 0;

    int n_levels = 6;
    m_save = new TWSave(n_levels);

    FILE *result = fopen("result.dat", "rb");
    int v[4];

    if(not result){
        printf("Não foi possível abrir o arquivo result.dat\n");
        exit(1);
    }

    fread(&v[0], sizeof(int), 5, result);

    int played_level = v[0];
    n_collectables = v[1];
    n_defeated_enemies = v[2];
    m_game_over = v[3];
    m_prev_level = v[4];

    system("rm result.dat");

    double max_col = m_save -> max_collectables(played_level);
    double percentage_col = (100.0 * n_collectables)/max_col;

    if(m_game_over){
        m_background = resources::get_texture(current_level + "/background-game-over.png");
        m_audio = "/opt/traveling-will/res/result/losing.wav";

        m_win = false;
        m_result = resources::get_texture(current_level + "/game-over.png");
        m_will_sprite = resources::get_texture(current_level + "/will-lose.png");
    }
    else{
        m_background = resources::get_texture(current_level + "/background.png");
        m_collectable = resources::get_texture(current_level + "/collectables.png");
        m_enemy = resources::get_texture(current_level + "/enemies.png");

        if(percentage_col >= MINIMUM_PERCENTAGE){
            m_win = true;
            m_audio = "/opt/traveling-will/res/" + to_string(played_level) + "/victory.wav";
            m_result = resources::get_texture(current_level + "/win.png");
            m_will_sprite = resources::get_texture(current_level + "/will-win.png");

            m_save -> set_cleared(played_level);
            m_save -> set_unlocked(played_level + 1);
        }else{
            m_win = false;
            m_audio = "/opt/traveling-will/res/result/losing.wav";
            m_result = resources::get_texture(current_level + "/lose.png");
            m_will_sprite = resources::get_texture(current_level + "/will-lose.png");
        }

        if( (n_collectables > m_save -> record_collectables(played_level)) ||
            (n_collectables == m_save -> record_collectables(played_level)
            && n_defeated_enemies > m_save -> record_enemies(played_level)) ){
            m_save -> set_record(played_level, n_collectables, n_defeated_enemies);
        }

        m_save->increase_times_played(played_level);
    }

    m_save -> update();

    numbers = resources::get_texture("numbers.png");

    m_buttons.clear();
    m_buttons.push_back(new TWButton("limbo", m_current_level, 680, 410, "limbo-botao.png", 104, 34));
    m_buttons.push_back(new TWButton("restart", m_current_level, 30, 410, "restart-botao.png", 354, 34));

    for(auto btn : m_buttons){
        add_child(btn);
    }

    event::register_listener(this);
}

TWResult::~TWResult(){
    event::unregister_listener(this);
}

bool TWResult::done() const{
    return m_done;
}

string TWResult::next() const{
    return m_next;
}

string TWResult::audio() const{
    return m_audio;
}

bool TWResult::on_event(const GameEvent&){
    return false;
}

void TWResult::update_self(unsigned now, unsigned){
    if(m_state == SELECTING){
        m_state = RUNNING;
        m_done = true;
    }

    if(m_start == -1){
        m_start = now;
    }

    m_sprite_counter += (now - m_start) * m_sprite_speed;
    if(m_sprite_counter > 5.9){
        m_sprite_counter -= 5.9;
    }

    m_start = now;
}

void TWResult::draw_self(Canvas *canvas, unsigned, unsigned){
    canvas->clear();

    canvas->draw(m_background.get(), 0, 0);
    canvas->draw(m_result.get(), 200, 200);

    if(not m_game_over){
        canvas->draw(m_collectable.get(), 200, 50);
        canvas->draw(m_enemy.get(), 200, 100);
    }

    if(not m_game_over)
        canvas->draw(m_will_sprite.get(), Rectangle(45 * (int)m_sprite_counter, 0, 45, 45), 75, 75);
    else
        canvas->draw(m_will_sprite.get(), Rectangle(45 * (int)m_sprite_counter, 0, 45, 45), 400, 75);

    int x_digit_col = 700;
    int counter_col = n_collectables;

    if(not m_game_over){
        do{
            canvas->draw(numbers.get(), Rectangle(23 * (counter_col % 10), 0, 23, 36), x_digit_col, 50);
            counter_col /= 10;
            x_digit_col -= 25;
        }while(counter_col);


        int x_digit_enemie = 700;
        int counter_enemies = n_defeated_enemies;

        do{
            canvas->draw(numbers.get(), Rectangle(23 * (counter_enemies % 10), 0, 23, 36), x_digit_enemie, 100);
            counter_enemies /= 10;
            x_digit_enemie -= 25;
        }while(counter_enemies);
    }
}

void TWResult::do_action(string label){
    if(label == "limbo"){
        m_next = "limbo";
        m_done = true;
    }

    if(label == "restart"){
        m_next = to_string(m_prev_level);
        m_done = true;
    }
}
