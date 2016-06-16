#ifndef TW_GAME_H
#define TW_GAME_H

#include <ijengine/game.h>
#include <ijengine/engine.h>
#include <ijengine/mouse_event.h>
#include <ijengine/system_event.h>
#include <ijengine/keyboard_event.h>
#include <ijengine/events_translator.h>

#include "tw_level_factory.h"

using namespace ijengine;

namespace ijengine
{
    namespace game_event
    {
        const unsigned GAME_EVENT_JUMP =            1 << 4;
        const unsigned GAME_EVENT_SLIDE_PRESSED =   1 << 5;
        const unsigned GAME_EVENT_SLIDE_RELEASED =  1 << 6;
        const unsigned GAME_EVENT_MENU_SELECT =     1 << 7;
        const unsigned GAME_MOUSE_CLICK =           1 << 8;
        const unsigned GAME_MOUSE_MOVEMENT =        1 << 9;
        const unsigned GAME_MOUSE_MOTION =          1 << 10;
        const unsigned GAME_EVENT_PUNCH =           1 << 11;
    }
}

class TWGame {
    public:
        TWGame(const string& title, int w, int h);
        ~TWGame();

        int run(const string& level_id);

    private:
        class Translator : public EventsTranslator {
            bool translate(GameEvent& to, const MouseEvent& from){
                to.set_timestamp(from.timestamp());
                to.set_property<double>("x", from.x());
                to.set_property<double>("y", from.y());

                if (from.state() == MouseEvent::MOTION)
                    to.set_id(game_event::GAME_MOUSE_MOTION);
                else
                    to.set_id(game_event::GAME_MOUSE_CLICK);

                return true;
            }

            bool translate(GameEvent& to, const SystemEvent& from){
                if(from.action() == SystemEvent::QUIT){
                    to.set_timestamp(from.timestamp());
                    to.set_id(game_event::QUIT);

                    return true;
                }

                return false;
            }


            virtual bool translate(GameEvent&, const JoystickEvent&){
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

                    case KeyboardEvent::X:
                        to.set_property<string>("punch", "x");
                        to.set_id(game_event::GAME_EVENT_PUNCH);
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
        TWLevelFactory m_level_factory;
};

#endif
