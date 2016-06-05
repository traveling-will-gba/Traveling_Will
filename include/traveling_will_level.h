#ifndef TRAVELING_WILL_LEVEL_H
#define TRAVELING_WILL_LEVEL_H

#include <string>
#include <memory>
#include <ijengine/level.h>
#include <ijengine/texture.h>
#include <ijengine/events_translator.h>
#include <ijengine/game_events_listener.h>
#include <ijengine/game_object.h>

#include <deque>
#include <vector>

using std::string;
using std::deque;
using std::shared_ptr;

using namespace ijengine;

class Button;
class Will;

class TravelingWillLevel : public Level, public GameEventsListener {
	public:
		~TravelingWillLevel(){}

	protected:		
        static const int GAME_MOUSE_CLICK =             1 << 8;
        static const int GAME_MOUSE_MOVEMENT =          1 << 9;
        static const int GAME_MOUSE_MOTION =            1 << 10;
        static const int BACK_BUTTON =                  0;

        typedef enum { RUNNING, JUMPING, SLIDING, FALLING, GAME_OVER, PUNCHING, SELECTING, NOTHING } State;

		State m_state;
		int m_audio_start, m_start;
		bool m_done;
		string m_current_level, m_audio, m_next;
		shared_ptr<Texture> m_background[3];
		vector< Button* > m_buttons;
};

#endif
