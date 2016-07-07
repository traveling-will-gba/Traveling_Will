#include "tw_portal_to_level.h"
#include "tw_platform.h"
#include "tw_limbo.h"

TWPortalToLevel::TWPortalToLevel(){

}

TWPortalToLevel::TWPortalToLevel(string current_level, int px, int py, int level_state, int level_collectables){
    m_sprite_speed = 1/270.0; //FIXME
    m_sprite_counter = 0;
    m_start = -1;

    m_height = 50;
    m_width = 40;
    m_y = py;
    m_x = px;
    m_num_col = level_collectables;
    m_texture = resources::get_texture("limbo/portal.png");
    m_numbers = resources::get_texture("numbers.png");
    m_col = resources::get_texture(current_level + "/collectable.png");
    m_background = resources::get_texture("limbo/limbo-baloon.png");

    m_active = true;
    m_info = false;
    this->set_priority(4);
    
    m_level = current_level;

    m_label = resources::get_texture("limbo/label" + m_level + ".png");

    m_bounding_box = Rectangle(m_x, m_y, m_width, m_height);
    physics::register_object(this);

    if(current_level == "1"){
        m_baloon_x = m_x - 50;
        m_baloon_y = m_y + 60;
    }
    else if(current_level == "2"){
        m_baloon_x = m_x - 50;
        m_baloon_y = m_y + 60;
    }
    else if(current_level == "3"){
        m_baloon_x = m_x - 50;
        m_baloon_y = m_y + 60;
    }
    else if(current_level == "4"){
        m_baloon_x = m_x - 160;
        m_baloon_y = m_y - 40;
    }
    else if(current_level == "5"){
        m_baloon_x = m_x - 50;
        m_baloon_y = m_y + 60;
    }
    else if(current_level == "6"){
        m_baloon_x = m_x - 50;
        m_baloon_y = m_y - 150;
    }
}

TWPortalToLevel::~TWPortalToLevel(){
    physics::unregister_object(this);
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
        m_info = true;
        p->set_next(m_level);
    }
}

void TWPortalToLevel::update_self(unsigned now, unsigned) {
    m_info = false;

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
    if(m_info){
        canvas->draw(m_background.get(), m_baloon_x, m_baloon_y);
        canvas->draw(m_label.get(), m_baloon_x + 20, m_baloon_y + 10);
        canvas->draw(m_col.get(), Rectangle(0, 0, 30, 30), m_baloon_x + 20, m_baloon_y + 70);

        int x_digit_col = m_baloon_x + 50;
        int counter_col = m_num_col;

        do{
            canvas->draw(m_numbers.get(), Rectangle(23 * (counter_col % 10), 0, 23, 36), x_digit_col + 60, m_baloon_y + 70);
            counter_col /= 10;
            x_digit_col -= 25;
        }while(counter_col);

    }

    if(m_active){
        canvas->draw(m_texture.get(), Rectangle(m_width * (int)m_sprite_counter, 0, m_width, m_height), m_x, m_y);
    }
}

