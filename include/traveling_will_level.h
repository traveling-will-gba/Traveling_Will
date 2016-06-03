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
		typedef enum { RUNNING, JUMPING, SLIDING, SELECTING, NOTHING, FALLING, GAME_OVER, PUNCHING } State;

		State m_state;
		int m_audio_counter;
		bool m_done;
		string m_current_level, m_audio, m_next;
		shared_ptr<Texture> m_background[3];
		vector< Button* > m_buttons;
};

#endif
