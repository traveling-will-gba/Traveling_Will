#ifndef TRAVELING_WILL_LEVEL_H
#define TRAVELING_WILL_LEVEL_H

#include <vector>
#include <string>
#include <memory>
#include <ijengine/level.h>
#include <ijengine/texture.h>
#include <ijengine/events_translator.h>
#include <ijengine/game_events_listener.h>

#include "traveling_will_level.h"

using std::string;
using std::shared_ptr;

using namespace ijengine;

class Button;

class TravelingWillPlayableLevel : public TravelingWillLevel{
    public:
        TravelingWillPlayableLevel(int r, int g, int b, const string& current_level, const string& next_level = "", const string audio_path = "", int audio_duration = -1);
        ~TravelingWillPlayableLevel();
};

#endif
