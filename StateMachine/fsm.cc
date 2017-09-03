#include "fsm.h"
#include <iostream>

void GameMode::OnCycle() {
	Exec(&GameState::OnCycle);
}

void GameMode::OnSelect() {
	Exec(&GameState::OnSelect);
}

void GameMode::Exec(mptr func) {
	if(current_state==EndState) {
		std::cout << "Not executing because in end state\n";
		return;
	}
	states[current_state]->OnExit(*this);
	current_state = (*states[current_state].*func)(*this);
	if(current_state==EndState) {
		std::cout << "Machine stoped\n";
		return;
	}
	states[current_state]->OnEntry(*this);
}
