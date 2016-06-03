#ifndef TRAVELING_WILL_LEVEL_H
#define TRAVELING_WILL_LEVEL_H

#include <string>
#include <memory>
#include <ijengine/level.h>
#include <ijengine/texture.h>
#include <ijengine/events_translator.h>
#include <ijengine/game_events_listener.h>
#include <ijengine/game_object.h>

#include <vector>

using std::string;
using std::shared_ptr;

using namespace ijengine;

class Button;

class TravelingWillLevel : public Level, public GameEventsListener{
	public:
		~TravelingWillLevel(){}

	protected:		
		static const int GAME_EVENT_JUMP =              1 << 4;
		static const int GAME_EVENT_SLIDE_PRESSED =     1 << 5;
		static const int GAME_EVENT_SLIDE_RELEASED =    1 << 6;
		static const int GAME_EVENT_MENU_SELECT =       1 << 7;
		static const int GAME_MOUSE_CLICK =             1 << 8;
		static const int GAME_MOUSE_MOVEMENT =          1 << 9;
		static const int GAME_MOUSE_MOTION =            1 << 10;
		static const int GAME_EVENT_PUNCH =             1 << 11;
		static const int NUMBER_OF_SCREENS =            12;
		static const int WILL_HEIGHT =                  45;
		static const int WILL_WIDTH =                   57;
		static const int COLLECTABLE_DIMENSION =        34;
		static const int COLLECTABLE_SIZE =             WILL_HEIGHT + COLLECTABLE_DIMENSION;
		static const int ENEMY_DIMENSION =              45;
		static const int ENEMY_SIZE =                   WILL_HEIGHT + ENEMY_DIMENSION;
		static const int BACK_BUTTON =                  0;


		typedef enum { RUNNING, JUMPING, SLIDING, SELECTING, NOTHING, FALLING, GAME_OVER, PUNCHING } State;

		State m_state;
		int m_audio_start, m_start;
		bool m_done;
		string m_current_level, m_audio, m_next;
		shared_ptr<Texture> m_background[3];
		vector< Button* > m_buttons;
};

#endif
