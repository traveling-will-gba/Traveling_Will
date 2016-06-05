#ifndef PLATFORM_H
#define PLATFORM_H

#include <iostream>
#include <memory>
#include "enemy.h"
#include "collectable.h"
#include <ijengine/game_object.h>
#include <ijengine/rectangle.h>
#include <ijengine/engine.h>

using namespace ijengine;
using std::shared_ptr;
using std::string;

class Platform : public GameObject {
    public:
        Platform();
        ~Platform();
        double x();
        double y();
        double height();
        double widht();
        void set_x(double px);
        void set_y(double py);

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);

    private:
        double m_x, m_y;
        double m_height, m_width;
        shared_ptr<Texture> m_sprite;
        Enemy m_enemy;
        Collectable m_collectable;
};

#endif
