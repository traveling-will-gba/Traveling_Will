#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <memory>
#include <ijengine/game_object.h>
#include <ijengine/rectangle.h>
#include <ijengine/engine.h>

using namespace ijengine;
using std::shared_ptr;
using std::string;

class Button : public GameObject {
    public:
        Button(int b_id, string level, double b_x, double b_y, string img, double b_w, double b_h);
        ~Button();
        double w();
        double h();
        double x();
        double y();
        int id();

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);        

    private:
        int m_id;
        string m_img, m_action, m_level;
        double m_x, m_y, m_img_h, m_img_w;
        shared_ptr<Texture> m_texture;
};

#endif
