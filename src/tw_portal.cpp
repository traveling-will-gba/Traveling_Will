#include "tw_portal.h"
#include "tw_platform.h"
#include "tw_limbo.h"
#include "tw_playable_level.h"

TWPortal::TWPortal(int px, int py, double speed, bool type){
	m_x_speed = speed;
    m_sprite_speed = 1/270.0; //FIXME
    m_sprite_counter = 0;
    m_start = -1;
	m_type = type;

    m_height = 77;
    m_width = 30;
    m_y = py;
    m_x = px;
    m_texture = resources::get_texture("portal.png");
    
    m_active = true;
    this->set_priority(5);
    
    m_bounding_box = Rectangle(m_x, m_y, m_width, m_height);
    physics::register_object(this);
}

TWPortal::~TWPortal(){
    //printf("Vamos destruir collectable\n");
    physics::unregister_object(this);
    //printf("Destruiu de boa\n");
}

double TWPortal::x(){ return m_x; }
double TWPortal::y(){ return m_y; }
double TWPortal::height(){ return m_height; }
double TWPortal::width(){ return m_width; }
shared_ptr<Texture> TWPortal::texture(){ return m_texture; }

void TWPortal::set_x(double cx) { m_x = cx; }
void TWPortal::set_y(double cy) { m_y = cy; }
void TWPortal::set_height(double ch) { m_height = ch; }    

void TWPortal::set_x_speed(double speed){ m_x_speed = speed; }

bool TWPortal::active() const{
    return m_active;
}

pair<double, double> TWPortal::direction() const{
    pair<double, double> p(0,0);
    return p;
}

const Rectangle& TWPortal::bounding_box() const{
    return m_bounding_box;
}

const list<Rectangle>& TWPortal::hit_boxes() const{
    return l;
}

void TWPortal::on_collision(const Collidable *, const Rectangle& where, const unsigned now, const unsigned last){
	if(m_type == END){
		if(auto p = dynamic_cast<TWPlayableLevel *>(this->parent())){
	        p->set_done(true);
	    }
	}
}

void TWPortal::update_self(unsigned now, unsigned) {
    if(m_start == -1){
        m_start = now;
    }

	m_x -= (now - m_start) * m_x_speed;
	printf("portal: %.2f\n", m_x);

    m_bounding_box = Rectangle(m_x, m_y, m_width, m_height);
    l.clear();
    l.insert(l.begin(), m_bounding_box);

    m_sprite_counter += (now - m_start) * m_sprite_speed;
    if(m_sprite_counter > 5.9){
        m_sprite_counter -= 5.9;
    }

    m_start = now;
}
void TWPortal::draw_self(Canvas* canvas, unsigned, unsigned) {
    ////printf("Entrando no draw de collectable\n");
/*    if(m_active)*/ canvas->draw(m_texture.get(), Rectangle(m_width * ((int) m_sprite_counter), 0, m_width, m_height), m_x, m_y);
    ////printf("Saindo do draw de collectable\n");
}

