#ifndef TRAVELING_WILL_LEVEL_H
#define TRAVELING_WILL_LEVEL_H

#include <string>
#include <ijengine/level.h>

using std::string;
using namespace ijengine;

class TravelingWillLevel : public Level {
public:
  TravelingWillLevel(int r, int g, int b, const string& next_level = "");

  bool done() const;
  string next() const;

protected:
  void update_self(unsigned now, unsigned last);
  void draw_self(Canvas *canvas, unsigned now, unsigned last);

private:
  int m_r, m_g, m_b;
  bool m_done;
  string m_next;
  int m_start;
};

#endif
