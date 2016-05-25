#include "traveling_will_level.h"
#include "traveling_will_level_factory.h"

TravelingWillLevel * TravelingWillLevelFactory::make_level(const string& level_id){
    if(level_id == "menu")
        return new TravelingWillLevel(255, 0, 0, "menu", "cutscene-intro", "res/menu/beat.wav", -1);
	else if(level_id == "cutscene-intro")
      return new TravelingWillLevel(255, 0, 0, "cutscene-intro", "1", "res/cutscene-intro/cutscene-intro.mp3", 28000);
    else if(level_id == "1")
        return new TravelingWillLevel(255, 0, 0, "1", "cutscene-end", "res/1/fase-1.mp3", 147000);
	else if(level_id == "cutscene-end")
        return new TravelingWillLevel(255, 0, 0, "cutscene-end", "", "res/cutscene-end/cutscene-end.mp3", 28000);
	else
        return nullptr;
}

void TravelingWillLevelFactory::release(Level *level){
    delete level;
}
