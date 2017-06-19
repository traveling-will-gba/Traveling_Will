#include "tw_game.h"

#include <iostream>
#include "../engine/include/game.h"
#include "../engine/include/exception.h"
#include "../engine/include/engine.h"
#include "../engine/include/events_translator.h"

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
