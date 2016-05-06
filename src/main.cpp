#include "traveling_will_game.h"

#include <iostream>
#include <ijengine/game.h>
#include <ijengine/exception.h>
#include <ijengine/engine.h>
#include <ijengine/events_translator.h>

using namespace ijengine;
using namespace std;

int main(){
  TravelingWillGame game("Traveling Will", 640, 480);

  return game.run("1");
}
