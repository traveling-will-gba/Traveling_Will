#include "traveling_will_level.h"
#include "traveling_will_level_factory.h"

TravelingWillLevel * TravelingWillLevelFactory::make_level(const string& level_id){
    if(level_id == "menu")
        return new TravelingWillLevel(255, 0, 0, "menu", "1", "res/beat.wav");
    else if(level_id == "1")
        return new TravelingWillLevel(255, 0, 0, "1", "", "res/introducao.mp3");
	else
        return nullptr;
}

void TravelingWillLevelFactory::release(Level *level){
    delete level;
}
