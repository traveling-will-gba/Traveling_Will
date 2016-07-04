#include "tw_result.h"

#include <ijengine/canvas.h>
#include <ijengine/engine.h>
#include <ijengine/keyboard_event.h>

#include <unistd.h>

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;
using namespace ijengine;

TWResult::TWResult(const string &current_level, const string& next_level, const string audio_path, int){
	m_current_level = current_level;
	m_audio = audio_path;
	m_next = next_level;
	m_done = false;
	m_state = NOTHING;
	m_start = -1;

	int n_levels = 6;
	m_save = new TWSave(n_levels);

	FILE *result = fopen("result.dat", "rb");
	int v[3];

	if(not result){
		printf("Não foi possível abrir o arquivo result.dat\n");
		exit(1);
	}

	fread(&v[0], sizeof(int), 3, result);

	n_collectables = v[1];
	n_defeated_enemies = v[2];

	system("rm result.dat");

	//Isso precisa ser pego do arquivo geral de salvamento
	int played_level = v[0];

	double max_col = m_save -> max_collectables(played_level);
	double percentage_col = (100.0 * n_collectables)/max_col;

	if(percentage_col >= MINIMUM_PERCENTAGE){
		final_texture = resources::get_texture(m_current_level + "/ganhou.png");
	}else{
		final_texture = resources::get_texture(m_current_level + "/perdeu.png");
	}

	numbers = resources::get_texture("numbers.png");

	m_buttons.push_back(new TWButton("limbo", m_current_level, 700, 410, "limbo-botao.png", 142, 50));

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

bool TWResult::on_event(const GameEvent& event){
	return false;
}

void TWResult::update_self(unsigned, unsigned){
	if(m_state == SELECTING){
		m_state = RUNNING;
		m_done = true;
	}
}

void TWResult::draw_self(Canvas *canvas, unsigned, unsigned){
	canvas->clear();

	canvas->draw(final_texture.get(), Rectangle(0, 0, 852, 480), 0, 0);

	int x_digit_col = 260;
	int counter_col = n_collectables;

	do{
		canvas->draw(numbers.get(), Rectangle(23 * (counter_col % 10), 0, 23, 36), x_digit_col, 17);
		counter_col /= 10;
		x_digit_col -= 25;
	}while(counter_col);


	int x_digit_enemie = 747;
	int counter_enemies = n_defeated_enemies;

	do{
		canvas->draw(numbers.get(), Rectangle(23 * (counter_enemies % 10), 0, 23, 36), x_digit_enemie, 17);
		counter_enemies /= 10;
		x_digit_enemie -= 25;
	}while(counter_enemies);

}

void TWResult::do_action(string label){
	if(label == "limbo"){
		m_next = "limbo";
		m_done = true;
	}
}
