#ifndef TRAVELING_WILL_LEVEL_H
#define TRAVELING_WILL_LEVEL_H

#include <string>
#include <memory>
#include <ijengine/level.h>
#include <ijengine/texture.h>
#include <ijengine/events_translator.h>
#include <ijengine/game_events_listener.h>

#include <vector>

using std::string;
using std::shared_ptr;

using namespace ijengine;

class Button;

class TravelingWillLevel : public Level, public GameEventsListener {
	public:
		~TravelingWillLevel(){}

		virtual bool on_event(const GameEvent& event) = 0;

	protected:
		virtual void update_self(unsigned now, unsigned last) = 0;
		virtual void draw_self(Canvas *canvas, unsigned now, unsigned last) = 0;
};

#endif
