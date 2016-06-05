#include "enemy.h"

Enemy::Enemy(){

}

Enemy::Enemy(double ey, double eh){

}

Enemy::~Enemy(){

}

double Enemy::x(){ return m_x; }
double Enemy::y(){ return m_y; }
double Enemy::height(){ return m_height; }
double Enemy::width(){ return m_width; }
int Enemy::type(){ return m_type; }
void Enemy::set_x(double ex){ m_x = ex; }
void Enemy::set_y(double ey){ m_y = ey; }

void Enemy::update_self(unsigned now, unsigned last){

}

void Enemy::draw_self(Canvas *canvas, unsigned now, unsigned last){
    canvas->draw(m_sprite.get(), m_x, m_y);
}

