#include "tw_portal_to_level.h"
#include "tw_platform.h"
#include "tw_limbo.h"

TWPortalToLevel::TWPortalToLevel(){

}

TWPortalToLevel::TWPortalToLevel(string current_level, int px, int py){
    m_sprite_speed = 1/270.0; //FIXME
    m_sprite_counter = 0;
    m_start = -1;

    m_height = 50;
    m_width = 40;
    m_y = py;
    m_x = px;
    m_texture = resources::get_texture("limbo/portal.png");
    
    m_active = true;
    this->set_priority(4);
    
    m_level = current_level;

    m_bounding_box = Rectangle(m_x, m_y, m_width, m_height);
    physics::register_object(this);

}

TWPortalToLevel::~TWPortalToLevel(){
    //printf("Vamos destruir collectable\n");
    physics::unregister_object(this);
    //printf("Destruiu de boa\n");
}

double TWPortalToLevel::x(){ return m_x; }
double TWPortalToLevel::y(){ return m_y; }
double TWPortalToLevel::height(){ return m_height; }
double TWPortalToLevel::width(){ return m_width; }
shared_ptr<Texture> TWPortalToLevel::texture(){ return m_texture; }

void TWPortalToLevel::set_x(double cx) { m_x = cx; }
void TWPortalToLevel::set_y(double cy) { m_y = cy; }
void TWPortalToLevel::set_height(double ch) { m_height = ch; }    

bool TWPortalToLevel::active() const{
    return m_active;
}

pair<double, double> TWPortalToLevel::direction() const{
    pair<double, double> p(0,0);
    return p;
}

const Rectangle& TWPortalToLevel::bounding_box() const{
    return m_bounding_box;
}

const list<Rectangle>& TWPortalToLevel::hit_boxes() const{
    return l;
}

void TWPortalToLevel::on_collision(const Collidable *, const Rectangle&, const unsigned, const unsigned){
    //printf("TWPortalToLevel colidiu em %.2f,%.2f em %u-%u\n", where.x(), where.y(), now, last);
    // m_active = false;

    if(auto p = dynamic_cast<TWLimbo *>(this->parent())){
        p->set_next(m_level);
    }
}

void TWPortalToLevel::update_self(unsigned now, unsigned) {
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
void TWPortalToLevel::draw_self(Canvas* canvas, unsigned, unsigned) {
    ////printf("Entrando no draw de collectable\n");
    if(m_active) canvas->draw(m_texture.get(), Rectangle(m_width * ((int) m_sprite_counter), 0, m_width, m_height), m_x, m_y);
    ////printf("Saindo do draw de collectable\n");
}

