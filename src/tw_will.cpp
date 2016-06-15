#include "tw_will.h"

static const int WILL_HEIGHT =              45;
static const int WILL_WIDTH =               45;
static const int EVENT_JUMP =               1 << 4;
static const int EVENT_SLIDE_PRESSED =      1 << 5;
static const int EVENT_SLIDE_RELEASED =     1 << 6;
static const int EVENT_PUNCH =              1 << 11;

TWWill::TWWill(double will_x, double will_y) : m_x(will_x), m_y(will_y) {
    m_sprite_counter = 0;
    this->set_state(RUNNING);
    m_height = WILL_HEIGHT;
    m_width = WILL_WIDTH;
    m_y_speed = 0;
    m_sprite_speed = 1/170.0;
    m_start = -1;
    m_is_punching = false;
    m_punch_counter = 0;
    m_bounding_box = Rectangle(m_x, m_y, m_width, m_height);

    m_sprite[RUNNING] = resources::get_texture("will/running.png");
    m_sprite[JUMPING] = resources::get_texture("will/jumping.png");
    m_sprite[SLIDING] = resources::get_texture("will/sliding.png");
    m_sprite[FALLING] = resources::get_texture("will/falling.png");
    m_sprite[PUNCHING] = resources::get_texture("will/punching.png");
    m_sprite[GAME_OVER] = resources::get_texture("will/gameover.png");
    this->set_priority(10);

    event::register_listener(this);
    physics::register_object(this);
}

TWWill::~TWWill(){
    physics::unregister_object(this);
    event::unregister_listener(this);
}

void TWWill::set_height(double will_h){ m_height = will_h; }
void TWWill::set_width(double will_w){ m_width = will_w; }
void TWWill::set_y(double will_y){ m_y = will_y; }
void TWWill::set_state(int will_state){ m_state = TWWill::State(will_state); }
void TWWill::set_y_speed(double will_speed){ m_y_speed = will_speed; }
void TWWill::update_y_speed(double speed_increment){ m_y_speed += speed_increment; }
int TWWill::state(){ return (int)m_state; }
double TWWill::x(){ return m_x; }
double TWWill::y(){ return m_y; }
double TWWill::speed(){ return m_y_speed; }

bool TWWill::on_event(const GameEvent& event){
    if(m_state != GAME_OVER){
        if(event.id() == EVENT_PUNCH && m_state != SLIDING && event.timestamp() - m_punch_counter > 230){
            m_is_punching = true;
            m_punch_counter = event.timestamp();
            return true;
        }

        if(event.id() == EVENT_JUMP && m_state == RUNNING){
            m_y_speed = -0.5;
            this->set_state(JUMPING);
            return true;
        }   

        if(event.id() == EVENT_SLIDE_PRESSED && m_state != JUMPING && m_state != FALLING){
            this->set_state(SLIDING);
            return true;
        }

        if(event.id() == EVENT_SLIDE_RELEASED && m_state == SLIDING){
            this->set_state(RUNNING);
            return true;
        }
    }

    return false;
}

bool TWWill::active() const{
    return true;
}

pair<double, double> TWWill::direction() const{
    pair<double, double> p(0,0);
    return p;
}

const Rectangle& TWWill::bounding_box() const{
    return m_bounding_box;
}

const list<Rectangle>& TWWill::hit_boxes() const{
    static list<Rectangle> l {m_bounding_box};
    return l;
}

void TWWill::on_collision(const Collidable *, const Rectangle& where, const unsigned now, const unsigned last){
    printf("TWWill colidiu em %.2f,%.2f em %u-%u\n", where.x(), where.y(), now, last);
}

void TWWill::update_self(unsigned now, unsigned){
    printf("Entrando em will update\n");
    if(m_start == -1){
        m_start = now;
    }

    m_sprite_counter += (now - m_start) * m_sprite_speed;
    m_y += (now - m_start) * m_y_speed;

    if(m_sprite_counter > 5.9){
        m_sprite_counter -= 5.9;
    }

    if(m_is_punching && (now - m_punch_counter) > 80){
        m_punch_counter = now;
        m_is_punching = false;
    }

    int slide_height = m_state == SLIDING ? 15 : 0;

    m_bounding_box = Rectangle(m_x, m_y + slide_height, m_width, m_height - slide_height);

    m_start = now;
    printf("Saindo de will update\n");
}

void TWWill::draw_self(Canvas *canvas, unsigned, unsigned){
    int will_state = m_is_punching ? PUNCHING : m_state;
    int slide_height = m_state == SLIDING ? 15 : 0;

    canvas->draw(m_sprite[will_state].get(), Rectangle(m_width * (int)m_sprite_counter, 0, m_width, m_height - slide_height), m_x, m_y + slide_height);
}
