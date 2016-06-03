#ifndef TRAVELING_WILL_PLAYABLE_LEVEL_H
#define TRAVELING_WILL_PLAYABLE_LEVEL_H

#include "traveling_will_level.h"

using std::string;
using std::shared_ptr;

using namespace ijengine;

class Button;

class TravelingWillPlayableLevel : public TravelingWillLevel{
    public:
        TravelingWillPlayableLevel(const string& current_level, const string& next_level = "", const string audio_path = "", 
		int audio_duration = -1);
        ~TravelingWillPlayableLevel();

		bool done() const;
		string next() const;
		string audio() const;

		bool on_event(const GameEvent& event);

	protected:
		void update_self(unsigned now, unsigned last);
		void draw_self(Canvas *canvas, unsigned now, unsigned last);

	private:
		private:
        bool m_is_punching, level_started, level_finished;
        int *enemy_type, *level_it;
        int *collectable, *enemy, m_will_enemy_type;
        int collectable_it, enemy_it, m_punch_counter;
        int m_audio_duration, m_audio_counter;
        int current_image, change, n_screens, n_collectables, n_enemies;
        double *platform_height;
        double start_cutscene_counter, final_cutscene_counter, m_cutscene_speed;

        double m_boss_x, m_boss_y;
        double m_x_speed, m_y_speed;
        double sprite_counter, m_sprite_speed;
        double *collectable_height, *enemy_height;
        double m_camera_x, m_camera_y, m_reverse_camera_x, m_reverse_camera_y;
        double m_will_x, m_will_y, m_will_floor, m_will_collectable, m_will_enemy;

        shared_ptr<Texture>  m_level[20], m_enemy[4], m_collectable, m_will[20], m_start_cutscene[30], m_end_cutscene[30], m_boss;
        shared_ptr<Texture> m_progress_bar[3], m_will_progress_bar, m_number, m_collectable_icon;
};

#endif
