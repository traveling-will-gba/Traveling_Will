#include "enemy.h"

Enemy::Enemy(){

}

Enemy::Enemy(double et, double eh){
    m_type = et;
    m_height = m_width = 45;
    m_y = 480 - eh;

    if(m_type == 1){
        m_texture = resources::get_texture("1/enemy1.png");
    }
    else{
        m_texture = resources::get_texture("1/enemy2.png");
    }
}

Enemy::~Enemy(){
    physics::unregister_object(this);
}

double Enemy::x(){ return m_x; }
double Enemy::y(){ return m_y; }
double Enemy::height(){ return m_height; }
double Enemy::width(){ return m_width; }
int Enemy::type(){ return m_type; }
shared_ptr<Texture> Enemy::texture(){ return m_texture; }

void Enemy::set_x(double ex){ m_x = ex; }
void Enemy::set_y(double ey){ m_y = ey; }
void Enemy::set_type(int et){ m_type = et; }
void Enemy::set_height(double eh){ m_height = eh; }

void Enemy::register_self(int current_x){
    m_bounding_box = Rectangle(current_x, m_y, m_width, m_height);
    physics::register_object(this);
}

bool Enemy::active() const{
    return true;
}

pair<double, double> Enemy::direction() const{
    pair<double, double> p(0,0);
    return p;
}

const Rectangle& Enemy::bounding_box() const{
    return m_bounding_box;
}

const list<Rectangle>& Enemy::hit_boxes() const{
    static list<Rectangle> l {m_bounding_box};
    return l;
}

void Enemy::on_collision(const Collidable *who, const Rectangle& where, const unsigned now, const unsigned last){
    printf("Enemy colidiu\n");
}

void Enemy::update_self(unsigned, unsigned) {
    m_bounding_box = Rectangle(m_x, m_y, m_width, m_height);
}
void Enemy::draw_self(Canvas * canvas, unsigned, unsigned){
    canvas->draw(m_texture.get(), Rectangle(m_width * 0, 0, m_width, m_height), m_x, m_y);
}

