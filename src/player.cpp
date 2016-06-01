#include "player.h"

static const int WILL_HEIGHT =              45;
static const int WILL_WIDTH =               57;
static const int EVENT_JUMP =               1 << 4;
static const int EVENT_SLIDE_PRESSED =      1 << 5;
static const int EVENT_SLIDE_RELEASED =     1 << 6;
static const int EVENT_PUNCH =              1 << 11;


Player::Player(double x, double y) : m_x(x), m_y(y) {
    m_sprite_counter = 0;
    m_state = RUNNING;
    m_height = 45;
    m_width = 57;
    m_y_speed = 0;

    m_sprite[RUNNING] = resources::get_texture("player/will-running.png");
    m_sprite[JUMPING] = resources::get_texture("player/will-jumping.png");
    m_sprite[SLIDING] = resources::get_texture("player/will-sliding.png");
    m_sprite[FALLING] = resources::get_texture("player/will-falling.png");
    m_sprite[PUNCHING] = resources::get_texture("player/will-punching.png");
    m_sprite[GAME_OVER] = resources::get_texture("player/will-gameover.png");
}

Player::~Player(){
}

void Player::set_height(double h){
    m_height = h;
}

void Player::set_width(double w){
    m_width = w;
}

void Player::set_y(double y){
    m_y = y;
}

void Player::set_state(int state){
    m_state = Player::State(state);
}

int Player::state(){
    return (int)m_state;
}

double Player::y(){
    return m_y;
}

bool Player::on_event(const GameEvent& event){

    if(m_state != GAME_OVER){
        if(event.id() == EVENT_PUNCH && m_state != SLIDING/* && event.timestamp() - m_punch_counter > 230*/){
            // m_is_punching = true;
            // m_punch_counter = event.timestamp();
            return true;
        }

        if(event.id() == EVENT_JUMP && m_state == RUNNING){
            m_y_speed = -0.5;
            m_state = JUMPING;
            return true;
        }   

        if(event.id() == EVENT_SLIDE_PRESSED && m_state != JUMPING && m_state != FALLING){
            m_state = SLIDING;
            return true;
        }

        if(event.id() == EVENT_SLIDE_RELEASED && m_state == SLIDING){
            m_state = RUNNING;
            return true;
        }
    }

    return false;
}

void Player::update_self(unsigned now, unsigned last){

}

void Player::draw_self(Canvas *canvas, unsigned now, unsigned last){
    // canvas->draw(
    //       m_will[m_is_punching ? PUNCHING : m_state].get(),
    //       Rectangle(WILL_WIDTH * (int) sprite_counter, 0, WILL_WIDTH, WILL_HEIGHT - 15*(m_state == SLIDING ? 1 : 0)),
    //       m_x,
    //       m_y + 15 * (m_state == SLIDING ? 1 : 0)
    // );

    canvas->draw(m_sprite[m_state].get(), Rectangle(m_sprite_counter, 0, m_width, m_height), m_x, m_y);
}
