#ifndef TW_TUTORIAL_H
#define TW_TUTORIAL_H

#include "tw_hud.h"

class TWTutorial : public TWHud {
    public:
        TWTutorial(string t_label, string texture_path, bool t_active = true);
        ~TWTutorial();
        string label();
        bool active();
        void set_active(bool act);

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);

    private:
        bool m_active;
        string m_label;
};


#endif