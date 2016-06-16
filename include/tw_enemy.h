#ifndef TW_ENEMY_H
#define TW_ENEMY_H

#include <ijengine/engine.h>
#include <ijengine/collidable.h>
#include <ijengine/canvas.h>
#include <ijengine/texture.h>
#include <ijengine/game_object.h>
#include <ijengine/events_translator.h>
#include <ijengine/game_events_listener.h>

using std::shared_ptr;
using namespace ijengine;

class TWEnemy : public GameObject, public Collidable {
    public:
        TWEnemy();
        TWEnemy(double et, double eh);
        ~TWEnemy();
        double x();
        double y();
        double height();
        double width();
        int type();
        shared_ptr<Texture> texture();

        void set_x(double ex);
        void set_y(double ey);
        void set_type(int et);
        void set_height(double eh);
        void register_self(int current_x);

        bool active() const;
        pair<double, double> direction() const;

        const Rectangle& bounding_box() const;
        const list<Rectangle>& hit_boxes() const;

        void on_collision(const Collidable *who, const Rectangle& where, const unsigned now, const unsigned last);

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);

    private:
        static const int INVALID = -10000000;
        double m_x, m_y;
        int m_type;
        double m_height, m_width;
        double m_sprite_counter, m_sprite_speed;
        int m_start;
        Rectangle m_bounding_box;
        // more sprites later(i.e. dying)
        shared_ptr<Texture> m_texture;
};

#endif
