#ifndef WILL_H
#define WILL_H

#include <ijengine/engine.h>
#include <ijengine/canvas.h>
#include <ijengine/texture.h>
#include <ijengine/game_object.h>
#include <ijengine/events_translator.h>
#include <ijengine/game_events_listener.h>

using std::shared_ptr;
using namespace ijengine;

class Will : public GameObject, public GameEventsListener {

    public:
        typedef enum { RUNNING, JUMPING, SLIDING, FALLING, GAME_OVER, PUNCHING } State;

        Will(double will_x, double will_y);
        ~Will();

        void set_height(double will_h);
        void set_width(double will_w);
        void set_y(double will_y);
        void set_state(int will_state);
        void set_y_speed(double will_speed);
        void update_y_speed(double speed_increment);
        int state();
        double x();
        double y();
        double speed();

        bool on_event(const GameEvent& event);

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);

    private:
        State m_state;
        double m_x, m_y, m_floor, m_y_speed;
        double m_height, m_width;
        double m_sprite_counter, m_sprite_speed;
        double m_punch_counter;
        bool m_is_punching;
        int m_start;
        shared_ptr<Texture> m_sprite[20];
};

#endif
