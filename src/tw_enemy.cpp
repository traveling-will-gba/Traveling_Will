#include "tw_enemy.h"
#include "tw_platform.h"

TWEnemy::TWEnemy(){

}

TWEnemy::TWEnemy(double et, double eh){
    m_sprite_speed = 1/85.0;
    m_sprite_counter = 0;
    m_start = -1;

    m_type = et;
    m_height = m_width = 45;
    m_y = 480 - eh;

    if(m_type == 1){
        m_texture = resources::get_texture("enemies/1.png");
    }
    else{
        m_texture = resources::get_texture("enemies/2.png");
    }

    m_active = false;
}

TWEnemy::~TWEnemy(){
    physics::unregister_object(this);
}

double TWEnemy::x(){ return m_x; }
double TWEnemy::y(){ return m_y; }
double TWEnemy::height(){ return m_height; }
double TWEnemy::width(){ return m_width; }
int TWEnemy::type() const{ return m_type; }
shared_ptr<Texture> TWEnemy::texture(){ return m_texture; }

void TWEnemy::set_x(double ex){ m_x = ex; }
void TWEnemy::set_y(double ey){ m_y = ey; }
void TWEnemy::set_type(int et){ m_type = et; }
void TWEnemy::set_height(double eh){ m_height = eh; }

void TWEnemy::register_self(int current_x){
    m_x = current_x;
    m_bounding_box = Rectangle(current_x, m_y, m_width, m_height);
    m_active = true;
    physics::register_object(this);
}

bool TWEnemy::active() const{
    return m_active;
}

pair<double, double> TWEnemy::direction() const{
    pair<double, double> p(0,0);
    return p;
}

const Rectangle& TWEnemy::bounding_box() const{
    return m_bounding_box;
}

const list<Rectangle>& TWEnemy::hit_boxes() const{
    return l;
}

void TWEnemy::on_collision(const Collidable *, const Rectangle&, const unsigned, const unsigned){
    //printf("TWWill colidiu em %.2f,%.2f em %u-%u\n", where.x(), where.y(), now, last);
    m_active = false;
}

void TWEnemy::update_self(unsigned now, unsigned) {
    if(m_start == -1){
        m_start = now;
    }

    m_bounding_box = Rectangle(m_x, m_y, m_width, m_height);
    l.clear();
    l.insert(l.begin(), m_bounding_box);

    m_sprite_counter += (now - m_start) * m_sprite_speed;
    if(m_sprite_counter > 5.9){
        m_sprite_counter -= 5.9;
    }

    m_start = now;
}
void TWEnemy::draw_self(Canvas * canvas, unsigned, unsigned){
    if(m_active) canvas->draw(m_texture.get(), Rectangle(m_width * ((int) m_sprite_counter), 0, m_width, m_height), m_x, m_y);
}

