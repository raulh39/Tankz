#include "fsm.h"
#include <tuple>

std::tuple<SelectingTankToFire, SelectingTarget> test;

template<typename T>
State& state() {
	return std::get<T>(test);
}

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

	state<SelectingTankToFire>().on_cycle(game);
}
