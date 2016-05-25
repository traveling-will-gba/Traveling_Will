#include "traveling_will_level.h"
#include "traveling_will_level_factory.h"

TravelingWillLevel * TravelingWillLevelFactory::make_level(const string& level_id){
    if(level_id == "menu")
        return new TravelingWillLevel(255, 0, 0, "menu", "1", "res/menu/beat.wav", -1);
    else if(level_id == "1")
        return new TravelingWillLevel(255, 0, 0, "1", "", "res/1/fase-1.mp3", 147000);
	else
        return nullptr;
}

void TravelingWillLevelFactory::release(Level *level){
    delete level;
}
