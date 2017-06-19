#include "tw_collectable.h"
#include "tw_platform.h"

TWCollectable::TWCollectable(){

}

TWCollectable::TWCollectable(std::string current_level, double ch){
    m_sprite_speed = 1/170.0;
    m_sprite_counter = 0;
    m_start = -1;

    m_height = m_width = 30;
    m_y = 480 - ch;
    m_texture = resources::get_texture(current_level + "/collectable.png");
    m_active = false;
}

TWCollectable::~TWCollectable(){
    //printf("Vamos destruir collectable\n");
    physics::unregister_object(this);
    //printf("Destruiu de boa\n");
}

double TWCollectable::x(){ return m_x; }
double TWCollectable::y(){ return m_y; }
double TWCollectable::height(){ return m_height; }
double TWCollectable::width(){ return m_width; }
shared_ptr<Texture> TWCollectable::texture(){ return m_texture; }

void TWCollectable::set_x(double cx) { m_x = cx; }
void TWCollectable::set_y(double cy) { m_y = cy; }
void TWCollectable::set_height(double ch) { m_height = ch; }    

void TWCollectable::register_self(int current_x){
    m_x = current_x;
    m_bounding_box = Rectangle(current_x, m_y, m_width, m_height);
    m_active = true;
    physics::register_object(this);
}

bool TWCollectable::active() const{
    return m_active;
}

pair<double, double> TWCollectable::direction() const{
    pair<double, double> p(0,0);
    return p;
}

const Rectangle& TWCollectable::bounding_box() const{
    return m_bounding_box;
}

const list<Rectangle>& TWCollectable::hit_boxes() const{
    return l;
}

void TWCollectable::on_collision(const Collidable *, const Rectangle&, const unsigned, const unsigned){
    //printf("TWCollectable colidiu em %.2f,%.2f em %u-%u\n", where.x(), where.y(), now, last);
    m_active = false;
}

void TWCollectable::update_self(unsigned now, unsigned) {
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
void TWCollectable::draw_self(Canvas* canvas, unsigned, unsigned) {
    ////printf("Entrando no draw de collectable\n");
    if(m_active) canvas->draw(m_texture.get(), Rectangle(m_width * ((int) m_sprite_counter), 0, m_width, m_height), m_x, m_y);
    ////printf("Saindo do draw de collectable\n");
}

