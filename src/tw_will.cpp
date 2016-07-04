#include "tw_will.h"
#include "tw_collectable.h"

static const int WILL_HEIGHT =              45;
static const int WILL_WIDTH =               45;
static const int EVENT_JUMP =               1 << 4;
static const int EVENT_DOWN_PRESSED =      1 << 5;
static const int EVENT_DOWN_RELEASED =     1 << 6;
static const int EVENT_PUNCH =              1 << 11;

TWWill::TWWill(double will_x, double will_y) : m_x(will_x), m_y(will_y) {
    m_sprite_counter = 0;
    this->set_state(RUNNING);
    m_height = WILL_HEIGHT;
    m_width = WILL_WIDTH;
    m_y_speed = 0;
    m_x_speed = 0;
    m_sprite_speed = 1/85.0;
    m_start = -1;
    m_is_punching = false;
    m_punch_counter = 0;
    m_collectables = 0;
    m_bounding_box = Rectangle(m_x, m_y, m_width, m_height);
    m_active_events = true;
    m_jump_counter = 0;

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
void TWWill::set_x(double will_x){ m_x = will_x; }
void TWWill::set_y(double will_y){ m_y = will_y; }
void TWWill::set_state(int will_state){ m_state = TWWill::State(will_state); }
void TWWill::set_y_speed(double will_speed){ m_y_speed = will_speed; }
void TWWill::set_x_speed(double will_speed){ m_x_speed = will_speed; }
void TWWill::set_m_active_events(bool ac_events){ m_active_events = ac_events; }
void TWWill::set_jump_counter(int jc){ m_jump_counter = jc; }
void TWWill::update_y_speed(double speed_increment){ m_y_speed += speed_increment; }
int TWWill::state(){ return (int)m_state; }
double TWWill::x(){ return m_x; }
double TWWill::y(){ return m_y; }
double TWWill::speed(){ return m_y_speed; }
int TWWill::collectables(){ return m_collectables; }
 
bool TWWill::on_event(const GameEvent& event){
    if(m_state != GAME_OVER && m_active_events){
        if(event.id() == EVENT_PUNCH && m_state != SLIDING && event.timestamp() - m_punch_counter > 230){
            m_is_punching = true;
            m_punch_counter = event.timestamp();
            return true;
        }

        if(event.id() == EVENT_JUMP){
            if(m_state == RUNNING || ((m_state == JUMPING || m_state == FALLING) && m_jump_counter < 3)){
                m_y_speed = -0.5;
                m_jump_counter++;
                this->set_state(JUMPING);
                return true;
            }
        }

        if(event.id() == EVENT_DOWN_PRESSED && m_state != JUMPING && m_state != FALLING){
            this->set_state(SLIDING);
            return false;
        }

        if(event.id() == EVENT_DOWN_RELEASED && m_state == SLIDING){
            this->set_state(RUNNING);
            return false;
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
    return l;
}

void TWWill::on_collision(const Collidable *who, const Rectangle&, const unsigned, const unsigned){
    //printf("TWWill colidiu em %.2f,%.2f em %u-%u\n", where.x(), where.y(), now, last);

    if(dynamic_cast<const TWCollectable *>(who)){
         m_collectables++;
     } //else if(auto q = dynamic_cast<const TWEnemy *>(who)){
    //     q->set_active(false);
    // }
}

void TWWill::update_self(unsigned now, unsigned){
    //printf("Entrando em will update\n");
    if(m_start == -1){
        m_start = now;
    }

    m_sprite_counter += (now - m_start) * m_sprite_speed;
    m_y += (now - m_start) * m_y_speed;
    m_x += (now - m_start) * m_x_speed;

    if(m_y < 0) m_y = 0;
    if(m_y > 480 - m_height) m_y = 480 - m_height;
    if(m_x < 0) m_x = 0;
    if(m_x > 852 - m_width) m_x = 852 - m_width;

    if(m_sprite_counter > 5.9){
        m_sprite_counter -= 5.9;
    }

    if(m_is_punching && (now - m_punch_counter) > 80){
        m_punch_counter = now;
        m_is_punching = false;
    }

    int slide_height = m_state == SLIDING ? 15 : 0;

    m_bounding_box = Rectangle(m_x, m_y + slide_height, m_width, m_height - slide_height);
    l.clear();
    l.insert(l.begin(), m_bounding_box);

    m_start = now;
    //printf("Saindo de will update\n");
}

void TWWill::draw_self(Canvas *canvas, unsigned, unsigned){
    int will_state = m_is_punching ? PUNCHING : m_state;
    int slide_height = m_state == SLIDING ? 15 : 0;

    canvas->draw(m_sprite[will_state].get(), Rectangle(m_width * (int)m_sprite_counter, 0, m_width, m_height - slide_height), m_x, m_y + slide_height);
}
