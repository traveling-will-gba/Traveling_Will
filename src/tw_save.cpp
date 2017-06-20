#include "tw_save.h"

#include <iostream>
#include <fstream>

using namespace std;

TWSave::TWSave(int n){
	n_levels = n;

	save = fopen("/opt/traveling-will/res/save.dat", "rb");

	if(not save){
		printf("Não foi possível abrir o arquivo save.dat\n");
		exit(1);
	}

	m_level_info = new Level_stat[n_levels + 1];

	fread(&m_level_info[1], sizeof(Level_stat), n_levels, save);
	fclose(save);
}

TWSave::~TWSave(){
	fclose(save);
	free(m_level_info);
}

bool TWSave::is_unlocked(int level){
	return m_level_info[level].unlocked;
}

bool TWSave::is_cleared(int level){
	return m_level_info[level].cleared;
}

void TWSave::set_unlocked(int level){
	m_level_info[level].unlocked = true;
}

void TWSave::set_cleared(int level){
	m_level_info[level].cleared = true;
}

void TWSave::set_record(int level, int n_cols, int n_enemies){
	m_level_info[level].record_collectables = n_cols;
	m_level_info[level].record_enemies = n_enemies;
}

void TWSave::increase_times_played(int level){
	m_level_info[level].times_played++;
}


int TWSave::max_collectables(int level){
	return m_level_info[level].max_collectables;
}

int TWSave::max_enemies(int level){
	return m_level_info[level].max_enemies;
}

int TWSave::record_collectables(int level){
	return m_level_info[level].record_collectables;
}

int TWSave::record_enemies(int level){
	return m_level_info[level].record_enemies;
}

int TWSave::times_played(int level){
	return m_level_info[level].times_played;
}

void TWSave::update(){
	save = fopen("/opt/traveling-will/res/save.dat", "wb");
	fwrite(&m_level_info[1], sizeof(Level_stat), n_levels, save);
	fclose(save);
}
