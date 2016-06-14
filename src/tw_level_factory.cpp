#include "tw_playable_level.h"
#include "tw_level_factory.h"

Level * TWLevelFactory::make_level(const string& level_id){
    if(level_id == "menu")
        return new TWMenu("menu", "cutscene-intro", "res/menu/menu.mp3", -1);
	else if(level_id == "cutscene-intro")
      return new TWCutscene("cutscene-intro", "1", "res/cutscene-intro/cutscene-intro.mp3", 28000);
    else if(level_id == "1")
        return new TWPlayableLevel("1", "2", "res/1/fase-1.mp3", 10000);
	else if(level_id == "2")
        return new TWPlayableLevel("2", "cutscene-end", "res/1/fase-1.mp3", 147000);
	else
        return nullptr;
}

void TWLevelFactory::release(Level *level){
    delete level;
}
