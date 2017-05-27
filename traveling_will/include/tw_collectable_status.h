#ifndef TW_COLLECTABLE_STATUS_H
#define TW_COLLECTABLE_STATUS_H

#include "tw_hud.h"

class TWCollectableStatus : public TWHud {
    public:
        TWCollectableStatus(string level);
        ~TWCollectableStatus();

        void update_collectable_counter(int counter);

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);        

    private:
        int m_collectable_counter; 
};


#endif