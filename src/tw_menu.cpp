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

	auto button = new TWButton("new-adventure", m_current_level, 50, 220, "menu-nova-aventura.png", 299, 34);
	m_buttons.push_back(button);
	button = new TWButton("missions", m_current_level, 50, 264, "menu-missoes.png", 167, 42);
	m_buttons.push_back(button);
	button = new TWButton("options", m_current_level, 50, 316, "menu-opcoes.png", 139, 51);
	m_buttons.push_back(button);
	button = new TWButton("exit", m_current_level, 50, 377, "menu-sair.png", 86, 34);
	m_buttons.push_back(button);

	for(auto btn : m_buttons){
		add_child(btn);
	}

	m_button_labels.push_back("new-adventure");
	m_button_labels.push_back("missions");
	m_button_labels.push_back("options");
	m_button_labels.push_back("exit");

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
	// m_buttons.clear();

	if(label == "new-adventure"){
		replace_texture_in_button(label, "menu-nova-aventura-on.png");

		m_next = "cutscene-intro";
		m_done = true;
	}
	
	if(label == "continue-adventure"){

	}

	if(label == "missions"){
		replace_texture_in_button(label, "menu-missoes-on.png");
	}

	if(label == "options"){
		replace_texture_in_button(label, "menu-opcoes-on.png");
	}

	if(label == "exit"){
		replace_texture_in_button(label, "menu-nova-aventura-on.png");
		exit(0);
	}
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

void TWMenu::replace_texture_in_button(string label, string texture){
	string old_texture;
	TWButton *btn = new TWButton();

	for(auto button : m_buttons){
		if(button->label() == label){
			old_texture = button->texture();
			btn = button;
			button->set_texture(texture);
			break;
		}
	}

	btn->set_texture(old_texture);
}
