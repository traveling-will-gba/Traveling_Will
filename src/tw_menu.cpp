#include "tw_menu.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/keyboard_event.h>

#include <unistd.h>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace ijengine;

TWMenu::TWMenu(const string &current_level, const string& next_level, const string audio_path, int){
	m_current_level = current_level;
	m_audio = audio_path;
	m_next = next_level;
	m_done = false;
	m_state = NOTHING;
	m_start = -1;

	m_background[0] = resources::get_texture(m_current_level + "/menu-fundo.png");
	m_background[1] = resources::get_texture(m_current_level + "/menu-titulo.png");

	m_buttons.clear();
	m_buttons.push_back(new TWButton("new-adventure", m_current_level, 50, 220, "menu-nova-aventura.png", 299, 34));
	m_buttons.push_back(new TWButton("continue-adventure", m_current_level, 50, 264, "menu-continuar-aventura.png", 409, 35));
	m_buttons.push_back(new TWButton("options", m_current_level, 50, 309, "menu-opcoes.png", 139, 51));
	m_buttons.push_back(new TWButton("exit", m_current_level, 50, 370, "menu-sair.png", 86, 34));

	for(auto btn : m_buttons){
		add_child(btn);
	}

	event::register_listener(this);
}

TWMenu::~TWMenu(){
	event::unregister_listener(this);
}

bool TWMenu::done() const{
	return m_done;
}

string TWMenu::next() const{
	return m_next;
}

string TWMenu::audio() const{
	return m_audio;
}

void TWMenu::do_action(string label){
	if(label == "new-adventure"){
		m_next = "cutscene-intro";
		m_done = true;
	}
	if(label == "continue-adventure"){
		m_next = "limbo";
		m_done = true;
	}
	if(label == "options"){
		for(auto btn : m_buttons){
			btn->set_active(false);
		}

	    m_buttons.push_back(new TWButton("back", m_current_level, 30, 410, "voltar-botao.png", 142, 50));
		m_buttons.push_back(new TWButton("volume", m_current_level, 150, 280, "opcoes/volume-botao.png", 189, 28));
		m_buttons.push_back(new TWButton("volume-bar", m_current_level, 370, 280, "opcoes/volume-barra.png", 225, 28));
		m_buttons.push_back(new TWButton("volume-ind", m_current_level, 575, 280, "opcoes/volume-indicador.png", 189, 28));

		for(auto btn : m_buttons){
			add_child(btn);
		}
	}
	if(label == "fullscreen-button"){
		printf("botao tela cheia\n");
	}
	if(label == "volume-bar"){
		auto indicator = m_buttons[m_buttons.size() - 1];
		auto bar = m_buttons[m_buttons.size() - 2];
		double old_x = indicator->x();
		double percentage = bar->percentage();
		double width = indicator->width();
		indicator->set_x(percentage - 14);
		double new_x = indicator->x();
        audio::set_audio_volume(1 - (575 - new_x)/225);
	}
	if(label == "back"){
		m_next = "menu";
		m_done = true;
	}
	if(label == "exit"){
		audio::play_sound_effect("res/effects/exit.wav");
		exit(0);
	}
}

bool TWMenu::on_event(const GameEvent&){
	return false;
}

void TWMenu::update_self(unsigned, unsigned){

	if(m_state == SELECTING){
		m_state = RUNNING;
		m_done = true;
	}
}

void TWMenu::draw_self(Canvas *canvas, unsigned, unsigned){
	canvas->clear();
	canvas->draw(m_background[0].get(), Rectangle(0, 0, 852, 480), 0, 0);
	canvas->draw(m_background[1].get(), Rectangle(0, 0, 852, 480), 0, 0);
}
