#include "../inc/game.h"
#include <iostream>

using namespace std;

Game::Game(){
	cout << "Creating object" << endl;
}

Game::~Game(){
	cout << "Destructing object" << endl;
}

bool Game::init(){
	cout << "Initializing game" << endl;
	return true;
}

bool Game::is_done(){
	cout << endl << "Not done yet" << endl;
	return false;
}

bool Game::get_input(){
	cout << "Getting input" << endl;
	return true;
}

bool Game::update(){
	cout << "Updating" << endl;
	return true;
}

bool Game::draw(){
	cout << "Drawing" << endl;
	return true;
}

void Game::quit(){
	cout << "Quitting game" << endl;
}
