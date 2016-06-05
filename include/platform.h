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
using namespace std;
using std::shared_ptr;
using std::string;

class Platform : public GameObject {
    public:
        Platform();
        Platform(double ph, double et, double eh, int ep, double ch, int cp);
        ~Platform();
        double x();
        double y();
        double height();
        double width();
        shared_ptr<Texture> texture();
        Collectable* collectable();
        Enemy* enemy();

        void set_x(double px);
        void set_y(double py);
        void set_height(double ph);

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);

    private:
        // insert level later
        double m_x, m_y;
        double m_height, m_width;
        static const int INVALID = -10000000;
        shared_ptr<Texture> m_sprite;
        Enemy *m_enemy;
        Collectable *m_collectable;
};

#endif
