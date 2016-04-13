#ifndef GAME_H
#define GAME_H

class Game {
	public:
		Game();
		~Game();

		bool init();
		bool is_done();
		bool get_input();
		bool update();
	  bool draw();
		void quit();
};

#endif
