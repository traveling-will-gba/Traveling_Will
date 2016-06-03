#include "menu.h"

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

Menu::Menu(const string &current_level, const string& next_level, const string audio_path, 
int audio_duration){
	m_current_level = current_level;
	m_audio = audio_path;
	m_next = next_level;
	m_done = false;
	m_state = NOTHING;

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

	for(auto btn : m_buttons){
		add_child(btn);
	}

	event::register_listener(this);
	}

Menu::~Menu(){
	event::unregister_listener(this);
}

bool Menu::done() const{
	return m_done;
}

string Menu::next() const{
	return m_next;
}

string Menu::audio() const{
	return m_audio;
}

bool Menu::on_event(const GameEvent& event){
	if(event.id() == GAME_EVENT_SLIDE_PRESSED){
		printf("PRA BAIXO\n");
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
						m_state = SELECTING;
						break;
					case 8:
						printf("FASE 2\n");
						break;
					case 9:
						printf("FASE 3\n");
						break;
					default:
						break;
				}

				return true;
			}

		}
	}

	return false;
}

void Menu::update_self(unsigned now, unsigned){
	if(m_state == SELECTING){
		m_state = RUNNING;
		m_done = true;
	}
}

void Menu::draw_self(Canvas *canvas, unsigned now, unsigned){
	canvas->clear();
	canvas->draw(m_background[0].get(), Rectangle(0, 0, 852, 480), 0, 0);
}
