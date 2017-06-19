#ifndef TW_PROGRESS_BAR_H
#define TW_PROGRESS_BAR_H

#include "tw_hud.h"

class TWProgressBar : public TWHud {
    public:
        TWProgressBar(string level, double length);
        ~TWProgressBar();

        void update_current_length(double length);

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);        

    private:
        double m_total_length, m_cur_length;
};


#endif