#include "traveling_will_level.h"

#include <ijengine/canvas.h>

using namespace std;
using namespace ijengine;

TravelingWillLevel::TravelingWillLevel(int r, int g, int b, const string& next_level)
  : m_r(r), m_g(g), m_b(b), m_done(false), m_next(next_level), m_start(-1) {
}

bool TravelingWillLevel::done() const{
  return m_done;
}

string TravelingWillLevel::next() const{
  return m_next;
}

void TravelingWillLevel::update_self(unsigned now, unsigned){
  if(m_start == -1)
    m_start = now;

  if(now - m_start > 1000)
    m_done = true;
}

void TravelingWillLevel::draw_self(Canvas *canvas, unsigned, unsigned){
  canvas->clear();
}
