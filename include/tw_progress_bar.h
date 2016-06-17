#ifndef TW_PROGRESS_BAR_H
#define TW_PROGRESS_BAR_H

#include "tw_hud.h"

class TWProgressBar : public TWHud {
    public:
        TWProgressBar(string level, int audio_duration);
        ~TWProgressBar();

        void update_audio_counter(int audio);

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);        

    private:
        int m_audio_counter, m_audio_duration;
};


#endif