#ifndef PLAYER_H
#define PLAYER_H

#include <ijengine/engine.h>
#include <ijengine/canvas.h>
#include <ijengine/texture.h>
#include <ijengine/game_object.h>
#include <ijengine/events_translator.h>
#include <ijengine/game_events_listener.h>

using std::shared_ptr;
using namespace ijengine;

class Player : public GameObject, public GameEventsListener {

    public:
        typedef enum { RUNNING, JUMPING, SLIDING, SELECTING, NOTHING, FALLING, GAME_OVER, PUNCHING } State;

        Player(double x, double y);
        ~Player();

        void set_height(double h);
        void set_width(double w);
        void set_y(double y);
        void set_state(int state);
        int state();
        double y();

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);
        bool on_event(const GameEvent& event);

    private:
        State m_state;
        double m_x, m_y, m_floor, m_y_speed;
        double m_height, m_width;
        double m_sprite_counter;
        shared_ptr<Texture> m_sprite[20];
};

#endif
