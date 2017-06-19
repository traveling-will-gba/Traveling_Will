#ifndef TW_CUTSCENE_H
#define TW_CUTSCENE_H

#include "tw_level.h"
#include "tw_button.h"

using std::string;
using std::shared_ptr;

using namespace ijengine;

class TWCutscene : public TWLevel{
	public:
		TWCutscene(const string& current_level, const string& next_level = "", const string audio_path = "", int audio_duration = -1);
		~TWCutscene();

		bool done() const;
		string next() const;
		string audio() const;
		void do_action(string label);

		bool on_event(const GameEvent& event);

	protected:
		void update_self(unsigned now, unsigned last);
		void draw_self(Canvas *canvas, unsigned now, unsigned last);

	private:
		double m_cutscene_speed;
		shared_ptr<Texture> m_start_cutscene[30], m_end_cutscene[30];
};

#endif
