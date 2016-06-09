#ifndef TW_BUTTON_H
#define TW_BUTTON_H

#include <memory>
#include <iostream>
#include <ijengine/game_object.h>
#include <ijengine/rectangle.h>
#include <ijengine/engine.h>

using namespace ijengine;
using std::shared_ptr;
using std::string;

class TWButton : public GameObject {
    public:
        TWButton(int b_id, string cur_level, double b_x, double b_y, string img, double b_w, double b_h, int able_draw);
        ~TWButton();
        double w();
        double h();
        double x();
        double y();
        string level();
        int id();
        int able_to_draw();
        void set_able_to_draw(int num);

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);        

    private:
        int m_id, m_able_draw;
        string m_img, m_action, m_level;
        double m_x, m_y, m_img_h, m_img_w;
        shared_ptr<Texture> m_texture;
};

#endif
