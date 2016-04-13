#include <iostream>
#include "../inc/game.h"

int main(){
	Game game;

	game.init();

	while(!game.is_done()){
		game.get_input();
		game.update();
		game.draw();
	}

	game.quit();

	return 0;
}
