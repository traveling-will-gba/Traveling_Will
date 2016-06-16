#include "tw_game.h"

#include <iostream>
#include <ijengine/game.h>
#include <ijengine/exception.h>
#include <ijengine/engine.h>
#include <ijengine/events_translator.h>

using namespace ijengine;
using namespace std;

int main(){

    try {
        TWGame game("Traveling Will", 852, 480);
        game.run("menu");
    }
    catch (Exception ex){
        cout << ex.what() << endl;
    }

    return 0;
}
