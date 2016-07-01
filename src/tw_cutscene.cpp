#include "tw_cutscene.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/keyboard_event.h>

#include <unistd.h>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace ijengine;

TWCutscene::TWCutscene(const string &current_level, const string& next_level, const string audio_path, int){
	m_current_level = current_level;
	m_audio = audio_path;
	m_next = next_level;
	m_done = false;
	m_state = NOTHING;
	m_start = -1;

	if(m_current_level == "cutscene-intro"){
		m_cutscene_speed = 1/300.0;

		for(int i=1; i<=9; i++){
			m_start_cutscene[i] = resources::get_texture(m_current_level + "/start_cutscene-" + to_string(i) + ".png");
		}

		// m_buttons.push_back(new TWButton(0, m_current_level, 700, 410, "pular-botao.png", 142, 50, 1));
	}
	else if(m_current_level == "cutscene-end"){
		m_cutscene_speed = 1/300.0;

		for(int i=1; i<=3; i++){
			m_end_cutscene[i] = resources::get_texture(m_current_level + "/end_cutscene-" + to_string(i) + ".png");
		}
	}

	// for(auto btn : m_buttons){
	// 	add_child(btn);
	// }
	
	event::register_listener(this);
}

TWCutscene::~TWCutscene(){
	event::unregister_listener(this);
}

bool TWCutscene::done() const{ return m_done; }
string TWCutscene::next() const{ return m_next; }
string TWCutscene::audio() const{ return m_audio; }

bool TWCutscene::on_event(const GameEvent& event){ return false; }

void TWCutscene::update_self(unsigned now, unsigned){
	if(m_start == -1){
        m_start = now;
        m_audio_start = m_start;
    }

	if(m_current_level == "cutscene-intro"){
        if((now - m_start) >= 28000){
            m_done = true;
        }
    }
	else if(m_current_level == "cutscene-end"){
		if((now - m_start) >= 9000){
			m_done = true;
		}
	}
}

void TWCutscene::draw_self(Canvas *canvas, unsigned now, unsigned){
	if(m_current_level == "cutscene-intro"){
		canvas->clear();
		canvas->draw(m_start_cutscene[1 + (now - m_start) / 3200].get(), Rectangle(0, 0, 852, 480), 0, 0);
	}
	else if(m_current_level == "cutscene-end"){
		canvas->clear();
		canvas->draw(m_end_cutscene[1 + (now - m_start) / 3200].get(), Rectangle(0, 0, 852, 480), 0, 0);
	}
}

