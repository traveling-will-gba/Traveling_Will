#include "tw_collectable.h"

TWCollectable::TWCollectable(){

}

TWCollectable::TWCollectable(double ch){
    m_height = m_width = 30;
    m_y = 480 - ch;
    m_texture = resources::get_texture("1/collectable.png");
}

TWCollectable::~TWCollectable(){
    physics::unregister_object(this);
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
    m_bounding_box = Rectangle(current_x, m_y, m_width, m_height);
    physics::register_object(this);
}

bool TWCollectable::active() const{
    return true;
}

pair<double, double> TWCollectable::direction() const{
    pair<double, double> p(0,0);
    return p;
}

const Rectangle& TWCollectable::bounding_box() const{
    return m_bounding_box;
}

const list<Rectangle>& TWCollectable::hit_boxes() const{
    static list<Rectangle> l {m_bounding_box};
    return l;
}

void TWCollectable::on_collision(const Collidable *who, const Rectangle& where, const unsigned now, const unsigned last){
    printf("TWCollectable colidiu\n");
}

void TWCollectable::update_self(unsigned, unsigned) {
    m_bounding_box = Rectangle(m_x, m_y, m_width, m_height);
}
void TWCollectable::draw_self(Canvas* canvas, unsigned, unsigned) {
    canvas->draw(m_texture.get(), Rectangle(m_width * 0, 0, m_width, m_height), m_x, m_y);
}

