#include "game.h"
#include <iostream>

int main()
{
	Game game;
	std::cout << "\nPressing Cycle()\n";
	game.on_cycle();
	std::cout << "\nPressing Select()\n";
	game.on_select();
	std::cout << "\nPressing Cycle()\n";
	game.on_cycle();
	std::cout << "\nPressing Cycle()\n";
	game.on_cycle();
	std::cout << "\nPressing Select()\n";
	game.on_select();
	std::cout << "\nPressing Cycle()\n";
	game.on_cycle();
	std::cout << "\nPressing Cycle()\n";
	game.on_cycle();
	std::cout << "\nPressing Select()\n";
	game.on_select();
	std::cout << "\nPressing Cycle()\n";
	game.on_cycle();
	std::cout << "\nPressing Select()\n";
	game.on_select();
	std::cout << "\nPressing Cycle()\n";
	game.on_cycle();
	std::cout << "\nPressing Select()\n";
	game.on_select();
}
