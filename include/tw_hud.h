#ifndef TW_HUD_H
#define TW_HUD_H

#include <map>
#include <vector>
#include <iostream>
#include <ijengine/engine.h>
#include <ijengine/canvas.h>
#include <ijengine/rectangle.h>
#include <ijengine/game_object.h>

using namespace ijengine;
using std::shared_ptr;
using std::string;
using std::map;

class TWHud : public GameObject {
    public:
        TWHud(string level, int type);
        ~TWHud(){}

        void set_attribute(string name, double value);

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);        

    private:
        typedef enum { COLLECTABLES, PROGRESS_BAR } Type;
        int m_type;
        string m_current_level;
        map<string, double> m_attributes;
        vector< shared_ptr<Texture> > m_textures;
};


#endif
