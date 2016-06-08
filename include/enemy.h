#ifndef ENEMY_H
#define ENEMY_H

#include <ijengine/engine.h>
#include <ijengine/canvas.h>
#include <ijengine/texture.h>
#include <ijengine/game_object.h>
#include <ijengine/events_translator.h>
#include <ijengine/game_events_listener.h>

using std::shared_ptr;
using namespace ijengine;

class Enemy : public GameObject {
    public:
        Enemy();
        Enemy(double et, double eh);
        ~Enemy();
        double x();
        double y();
        double height();
        double width();
        int type();
        int present();
        shared_ptr<Texture> texture();

        void set_x(double ex);
        void set_y(double ey);
        void set_type(int et);
        void set_height(double eh);
        void set_present(int ep);

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);

    private:
        static const int INVALID = -10000000;
        double m_x, m_y;
        int m_type, m_present;
        double m_height, m_width;
        double m_sprite_counter, m_sprite_speed;
        int m_start;
        // more sprites later(i.e. dying)
        shared_ptr<Texture> m_texture;
};

#endif
