#ifndef TW_RESULT_H
#define TW_RESULT_H

#include "tw_level.h"
#include "tw_button.h"
#include "tw_save.h"
#include "tw_will.h"

using std::string;
using std::shared_ptr;

using namespace ijengine;

class TWResult : public TWLevel{
    public:
        TWResult(const string& current_level, const string& next_level = "", const string audio_path = "", int audio_duration = -1);
        ~TWResult();

        bool done() const;
        string next() const;
        string audio() const;

        void do_action(string label);

        bool on_event(const GameEvent& event);

    protected:
        void update_self(unsigned, unsigned last);
        void draw_self(Canvas *canvas, unsigned, unsigned last);

    private:
        double MINIMUM_PERCENTAGE = 75.0;
        double m_sprite_counter, m_sprite_speed;

        int n_collectables, n_defeated_enemies, m_start;
        bool m_win;

        shared_ptr<Texture> numbers;
        shared_ptr<Texture> m_background, m_collectable;
        shared_ptr<Texture> m_will_sprite, m_enemy, m_result;

        TWSave *m_save;
};

#endif
