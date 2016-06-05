#include "enemy.h"

Enemy::Enemy(){

}

Enemy::Enemy(double et, double eh, int ep){
    m_type = et;
    m_height = m_width = 45;
    m_y = 480 - eh;
    m_present = ep;

    if(m_type == 1)
        m_sprite = resources::get_texture("1/enemy1.png");
    else
        m_sprite = resources::get_texture("1/enemy2.png");
}

Enemy::~Enemy(){

}

double Enemy::x(){ return m_x; }
double Enemy::y(){ return m_y; }
double Enemy::height(){ return m_height; }
double Enemy::width(){ return m_width; }
int Enemy::type(){ return m_type; }
int Enemy::present(){ return m_present; }
shared_ptr<Texture> Enemy::texture(){ return m_sprite; }

void Enemy::set_x(double ex){ m_x = ex; }
void Enemy::set_y(double ey){ m_y = ey; }
void Enemy::set_type(int et){ m_type = et; }
void Enemy::set_height(double eh){ m_height = eh; }
void Enemy::set_present(int ep){ m_present = ep; }

void Enemy::update_self(unsigned, unsigned) {}
void Enemy::draw_self(Canvas*, unsigned, unsigned) {}

