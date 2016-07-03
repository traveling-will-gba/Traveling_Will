#ifndef TW_PLAYABLE_LEVEL_H
#define TW_PLAYABLE_LEVEL_H

#include "tw_will.h"
#include "tw_level.h"
#include "tw_button.h"
#include "tw_platform.h"
#include "tw_progress_bar.h"
#include "tw_collectable_status.h"
#include "tw_portal.h"

using std::string;
using std::shared_ptr;

using namespace ijengine;

class TWPlayableLevel : public TWLevel{
    public:
        TWPlayableLevel(const string& current_level, const string& next_level = "", const string audio_path = "", 
		int audio_duration = -1);
        ~TWPlayableLevel();

		bool done() const;
		string next() const;
		string audio() const;

		bool on_event(const GameEvent& event);

		void set_done(bool done);

	protected:
		void update_self(unsigned now, unsigned last);
		void draw_self(Canvas *canvas, unsigned now, unsigned last);

	private:
        static const int GAME_EVENT_JUMP =				1 << 4;
        static const int GAME_EVENT_DOWN_PRESSED =		1 << 5;
        static const int GAME_EVENT_DOWN_RELEASED =		1 << 6;
        static const int GAME_EVENT_MENU_SELECT =       1 << 7;
        static const int GAME_EVENT_PUNCH =             1 << 11;
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
		static const int END =							1;
		static const int START = 						0;

        bool m_is_punching, level_started, level_finished;
        
        int m_audio_duration, m_audio_counter;
        int n_screens, m_punch_counter;
        int n_collectables, n_enemies;
        int m_cur_collectable_it, m_cur_enemy_it, n_backgrounds;
        
        deque<int> enemy_type, collectable, enemy;
        deque<double> collectable_height, enemy_height, platform_height;
        deque<TWPlatform *> platforms;

        double m_x_speed, m_y_speed;
        double sprite_counter, m_sprite_speed;
        double m_camera_x, m_camera_y, m_reverse_camera_x, m_reverse_camera_y;
        double m_floor;

        TWWill *m_will;
        TWCollectable *m_cur_collectable;
        TWEnemy *m_cur_enemy;
        TWProgressBar *m_progress_bar;
        TWCollectableStatus *m_collectable_status;
		TWPortal *m_portal_start, *m_portal_end;


        shared_ptr<Texture> m_floor_texture;

        void test_floor(unsigned now);
        void update_counters(unsigned now);
        void update_platforms_position();
};

#endif
