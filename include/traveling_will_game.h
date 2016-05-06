#ifndef TRAVELING_WILL_GAME_H
#define TRAVELING_WILL_GAME_H

#include <ijengine/game.h>
#include <ijengine/engine.h>
#include <ijengine/events_translator.h>

#include "traveling_will_level_factory.h"

using namespace ijengine;

class TravelingWillGame {
public:
    TravelingWillGame(const string& title, int w, int h);
    ~TravelingWillGame();

    int run(const string& level_id);
private:
    Game m_game;
    Engine m_engine;
    EventsTranslator m_translator;
    TravelingWillLevelFactory m_level_factory;
};

#endif
