#ifndef MENU_H
#define MENU_H

#include "traveling_will_level.h"
#include "button.h"

using std::string;
using std::shared_ptr;

using namespace ijengine;

class Button;

class Menu : public TravelingWillLevel{
	public:
		Menu(const string& current_level, const string& next_level = "", const string audio_path = "", int audio_duration = -1);
		~Menu();

		bool done() const;
		string next() const;
		string audio() const;

		bool on_event(const GameEvent& event);

	protected:
		void update_self(unsigned, unsigned last);
		void draw_self(Canvas *canvas, unsigned, unsigned last);
};

#endif
