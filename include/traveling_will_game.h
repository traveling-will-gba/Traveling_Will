#ifndef TRAVELING_WILL_GAME_H
#define TRAVELING_WILL_GAME_H

#include <ijengine/game.h>
#include <ijengine/engine.h>
#include <ijengine/mouse_event.h>
#include <ijengine/system_event.h>
#include <ijengine/keyboard_event.h>
#include <ijengine/events_translator.h>

#include "traveling_will_level_factory.h"

using namespace ijengine;

namespace ijengine
{
    namespace game_event
    {
        const unsigned MOVEMENT = GameEvent::assign_id();
        const unsigned MOTION = GameEvent::assign_id();
        const unsigned GAME_EVENT_JUMP =            1 << 4;
        const unsigned GAME_EVENT_SLIDE_PRESSED =   1 << 5;
        const unsigned GAME_EVENT_SLIDE_RELEASED =  1 << 6;
        const unsigned GAME_EVENT_MENU_SELECT =     1 << 7;
    }
}

class TravelingWillGame {
    public:
        TravelingWillGame(const string& title, int w, int h);
        ~TravelingWillGame();

        int run(const string& level_id);

    private:
        class Translator : public EventsTranslator {
            bool translate(GameEvent& to, const MouseEvent& from){
                return false;
            }

            bool translate(GameEvent& to, const SystemEvent& from){
                if(from.action() == SystemEvent::QUIT){
                    to.set_timestamp(from.timestamp());
                    to.set_id(game_event::QUIT);

                    return true;
                }

                return false;
            }

            virtual bool translate(GameEvent& to, const KeyboardEvent& from){
                to.set_timestamp(from.timestamp());

                bool done = true;

                switch(from.key()){
                    case KeyboardEvent::SPACE:
                        to.set_property<string>("jump", "space");
                        to.set_id(game_event::GAME_EVENT_JUMP);
                        break;

                    case KeyboardEvent::C:
                        to.set_property<string>("select", "c");
                        to.set_id(game_event::GAME_EVENT_MENU_SELECT);
                        break;

                    case KeyboardEvent::DOWN:
                        to.set_property<string>("slide", "down");

                        if(from.state() == KeyboardEvent::PRESSED)
                            to.set_id(game_event::GAME_EVENT_SLIDE_PRESSED);
                        else if(from.state() == KeyboardEvent::RELEASED)
                            to.set_id(game_event::GAME_EVENT_SLIDE_RELEASED);

                        break;

                    default:
                        done = false;
                }

                return done;
            }
        };

        Game m_game;
        Engine m_engine;
        Translator m_translator;
        TravelingWillLevelFactory m_level_factory;
};

#endif
