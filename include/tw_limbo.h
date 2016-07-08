#ifndef TW_LIMBO_H
#define TW_LIMBO_H

#include "tw_will.h"
#include "tw_save.h"
#include "tw_level.h"
#include "tw_button.h"
#include "tw_portal_to_level.h"

using std::string;
using std::shared_ptr;

using namespace ijengine;

class TWLimbo : public TWLevel{
    public:
        TWLimbo(const string& current_level, const string& next_level = "", const string audio_path = "", 
        int audio_duration = -1);
        ~TWLimbo();

        void set_next(string next_level);

        bool done() const;
        string next() const;
        string audio() const;
        void do_action(string label);

        bool on_event(const GameEvent& event);

    protected:
        void update_self(unsigned now, unsigned last);
        void draw_self(Canvas *canvas, unsigned now, unsigned last);

    private:
		static const int GAME_EVENT_JUMP =              1 << 4;
        static const int GAME_EVENT_DOWN_PRESSED =      1 << 5;
        static const int GAME_EVENT_DOWN_RELEASED =     1 << 6;
        static const int GAME_EVENT_MENU_SELECT =       1 << 7;
        static const int GAME_EVENT_PUNCH =             1 << 11;
        static const int GAME_EVENT_UP_PRESSED =        1 << 12;
        static const int GAME_EVENT_UP_RELEASED =       1 << 13;
        static const int GAME_EVENT_LEFT_PRESSED =      1 << 14;
        static const int GAME_EVENT_LEFT_RELEASED =     1 << 15;
        static const int GAME_EVENT_RIGHT_PRESSED =     1 << 16;
        static const int GAME_EVENT_RIGHT_RELEASED =    1 << 17;
        static const int GAME_EVENT_ENTER =             1 << 18;

        static const int NUMBER_OF_SECTIONS =           25;
        static const int WILL_HEIGHT =                  45;
        static const int WILL_WIDTH =                   45;
        static const int COLLECTABLE_DIMENSION =        30;
        static const int COLLECTABLE_SIZE =             WILL_HEIGHT + COLLECTABLE_DIMENSION;
        static const int ENEMY_DIMENSION =              45;
        static const int ENEMY_SIZE =                   WILL_HEIGHT + ENEMY_DIMENSION;
        static const int INVALID =                      -10000000;
        static const int COLLECTABLE =                  0;
        static const int ENEMY =                        1; 
        static const int PLATFORM_SIZE =                36;
        static const int STOPPED =                      6;

        int m_audio_duration, m_audio_counter;

        double m_x_speed, m_y_speed;
        double sprite_counter, m_sprite_speed;
        double m_camera_x, m_camera_y, m_reverse_camera_x, m_reverse_camera_y;
        double m_floor;

        bool on_portal;

        TWWill *m_will;
        TWPortalToLevel *m_portal[10];
		TWSave *m_save;

        shared_ptr<Texture> m_background_texture;
};

#endif
