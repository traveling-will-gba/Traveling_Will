#ifndef TW_PLATFORM_H
#define TW_PLATFORM_H

#include "tw_enemy.h"
#include "tw_collectable.h"

#include <memory>
#include <iostream>
#include <ijengine/engine.h>
#include <ijengine/rectangle.h>
#include <ijengine/game_object.h>

using namespace ijengine;
using namespace std;
using std::shared_ptr;
using std::string;

class TWPlatform : public GameObject {
    public:
        TWPlatform();
        TWPlatform(string current_level, double ph, double et, double eh, int ep, double ch, int cp);
        ~TWPlatform();
        double x();
        double y();
        double height();
        double width();
        shared_ptr<Texture> texture();
        TWCollectable* collectable();
        TWEnemy* enemy();

        void set_x(double px);
        void set_y(double py);
        void set_height(double ph);
        void remove(int option);
        void register_objects(int m_x);
		void create_collectable(string current_level, int c_y);

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);

    private:
        // insert level later
        double m_x, m_y;
        double m_height, m_width;
        static const int INVALID = -10000000;
        static const int COLLECTABLE = 0;
        shared_ptr<Texture> m_texture, m_floor;
        TWEnemy *m_enemy;
        TWCollectable *m_collectable;
};

#endif
