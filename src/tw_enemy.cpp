#include "tw_enemy.h"
#include "tw_platform.h"

TWEnemy::TWEnemy(){

}

TWEnemy::TWEnemy(double et, double eh){
    m_sprite_speed = 1/170.0;
    m_sprite_counter = 0;
    m_start = -1;

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

TWEnemy::~TWEnemy(){
    physics::unregister_object(this);
}

double TWEnemy::x(){ return m_x; }
double TWEnemy::y(){ return m_y; }
double TWEnemy::height(){ return m_height; }
double TWEnemy::width(){ return m_width; }
int TWEnemy::type(){ return m_type; }
shared_ptr<Texture> TWEnemy::texture(){ return m_texture; }

void TWEnemy::set_x(double ex){ m_x = ex; }
void TWEnemy::set_y(double ey){ m_y = ey; }
void TWEnemy::set_type(int et){ m_type = et; }
void TWEnemy::set_height(double eh){ m_height = eh; }

void TWEnemy::register_self(int current_x){
    m_x = current_x;
    m_bounding_box = Rectangle(m_x, m_y, m_width, m_height);
    physics::register_object(this);
}

bool TWEnemy::active() const{
    return true;
}

pair<double, double> TWEnemy::direction() const{
    pair<double, double> p(-5/19.0,0);
    return p;
}

const Rectangle& TWEnemy::bounding_box() const{
    return m_bounding_box;
}

const list<Rectangle>& TWEnemy::hit_boxes() const{
    static list<Rectangle> l {m_bounding_box};
    return l;
}

void TWEnemy::on_collision(const Collidable *, const Rectangle& where, const unsigned now, const unsigned last){
    printf("TWWill colidiu em %.2f,%.2f em %u-%u\n", where.x(), where.y(), now, last);
    m_x = -100;

    //((TWPlatform *) parent())->remove(1);
    // if( TWPlatform * plat = dynamic_cast<TWPlatform *>(parent()) ){
    //     plat->remove(1);
    // }
}

void TWEnemy::update_self(unsigned now, unsigned) {
    if(m_start == -1){
        m_start = now;
    }

    m_bounding_box = Rectangle(m_x, m_y, m_width, m_height);

    m_sprite_counter += (now - m_start) * m_sprite_speed;
    if(m_sprite_counter > 5.9){
        m_sprite_counter -= 5.9;
    }

    m_start = now;
}
void TWEnemy::draw_self(Canvas * canvas, unsigned, unsigned){
    canvas->draw(m_texture.get(), Rectangle(m_width * ((int) m_sprite_counter), 0, m_width, m_height), m_x, m_y);
}

