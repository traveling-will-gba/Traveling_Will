#include <iostream>
#include <cstdio>

using namespace std;

typedef struct _ls{
	bool unlocked = false;
	bool cleared = false;
	int max_collectables = 0;
	int max_enemies = 0;
	int record_collectables = 0;
	int record_enemies = 0;
	int times_played = 0;
}Level_stat;

int main(){
	Level_stat level_info[1000];
	int level_id = 1;

	FILE *level;

	while((level = fopen((to_string(level_id) + "/level_design.txt").c_str(), "r")) && level){
		int time, n_blocks, n_background;

		fscanf(level, "%d %d %d", &time, &n_blocks, &n_background);

		int n_collectables = 0, n_enemies = 0;
		while(not feof(level)){
			int height_plat, have_col, have_enemie, height_col, type_enemie, height_enemie;				

			fscanf(level, "%d %d", &height_plat, &have_enemie);

			if(have_enemie){
				fscanf(level, "%d %d", &type_enemie, &height_enemie);

				if(type_enemie == 2)
					n_enemies++;
			}

			fscanf(level, "%d", &have_col);

			if(have_col){
				fscanf(level, "%d", &height_col);
				n_collectables++;
			}
		}

		if(level_id == 1)
			level_info[level_id].unlocked = true;

		level_info[level_id].max_collectables = n_collectables;
		level_info[level_id].max_enemies = n_enemies;

		fclose(level);
		level_id++;
	}

	FILE *save = fopen("save.dat", "wb");

	fwrite(&level_info[1], sizeof(Level_stat), level_id-1, save);

	fclose(save);

	return 0;
}
