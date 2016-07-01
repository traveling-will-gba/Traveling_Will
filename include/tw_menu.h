#ifndef TW_MENU_H
#define TW_MENU_H

#include "tw_level.h"
#include "tw_button.h"

using std::string;
using std::shared_ptr;

using namespace ijengine;

class TWMenu : public TWLevel {
	public:
		TWMenu(const string& current_level, const string& next_level = "", const string audio_path = "", int audio_duration = -1);
		~TWMenu();

		bool done() const;
		string next() const;
		string audio() const;
		void do_action(string label);

		bool on_event(const GameEvent& event){}

		void replace_texture_in_button(string label, string texture);

	protected:
		void update_self(unsigned, unsigned last);
		void draw_self(Canvas *canvas, unsigned, unsigned last);

	private:
		vector<string> m_button_labels;
};

#endif
