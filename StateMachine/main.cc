#include "fsm.h"

int main()
{
	FSM game;
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

