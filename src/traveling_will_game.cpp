#include "traveling_will_game.h"

#include <ijengine/engine.h>
#include <ijengine/system_event.h>

TravelingWillGame::TravelingWillGame(const string& title, int w, int h)
    : m_game(title, w, h), m_engine(), m_level_factory(){

    event::register_translator(&m_translator);
    level::register_factory(&m_level_factory);
    resources::set_textures_dir("res");
}

TravelingWillGame::~TravelingWillGame(){
    level::unregister_factory();
    event::unregister_translator(&m_translator);
}

int TravelingWillGame::run(const string& level_id){
    return m_game.run(level_id);
}
