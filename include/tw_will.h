#ifndef TW_WILL_H
#define TW_WILL_H

#include <ijengine/engine.h>
#include <ijengine/collidable.h>
#include <ijengine/canvas.h>
#include <ijengine/texture.h>
#include <ijengine/game_object.h>
#include <ijengine/events_translator.h>
#include <ijengine/game_events_listener.h>

using std::shared_ptr;
using namespace ijengine;

class TWWill : public GameObject, public GameEventsListener, public Collidable {

    public:
        typedef enum { RUNNING, JUMPING, SLIDING, FALLING, GAME_OVER, PUNCHING, STOPPED } State;

        TWWill(double will_x, double will_y, int initial_state = RUNNING);
        ~TWWill();

        void set_height(double will_h);
        void set_width(double will_w);
        void set_x(double will_x);
        void set_y(double will_y);
        void set_state(int will_state);
        void set_y_speed(double will_speed);
        void set_x_speed(double will_speed);
        void set_m_active_events(bool ac_events);
        void set_jump_counter(int jc);
        void update_y_speed(double speed_increment);
        int state();
        double x();
        double y();
        double speed();
        double x_speed();
        int collectables();

        bool on_event(const GameEvent& event);

        bool active() const;
        pair<double, double> direction() const;

        const Rectangle& bounding_box() const;
        const list<Rectangle>& hit_boxes() const;

        void on_collision(const Collidable *who, const Rectangle& where, const unsigned now, const unsigned last);

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);

    private:
        State m_state;
        double m_x, m_y, m_floor, m_y_speed, m_x_speed;
        double m_height, m_width;
        double m_sprite_counter, m_sprite_speed;
        double m_punch_counter;
        bool m_is_punching, m_active_events;
        int m_start, m_collectables, m_jump_counter;
        Rectangle m_bounding_box;
        list<Rectangle> l;
        shared_ptr<Texture> m_sprite[20];
};

#endif
