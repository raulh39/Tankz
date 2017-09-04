#pragma once

#include <memory>

namespace idea2 {

class GameMode
{
public:

	class State{
	public:
		virtual ~State() {}

		virtual State* OnCycle()  { return this; }
		virtual State* OnSelect() { return this; }
	};

	GameMode(State *init_state) : current_state(init_state) {}

	//Events:
	void OnCycle()  { Exec(&State::OnCycle); }
	void OnSelect() { Exec(&State::OnSelect); }

private:
	State* current_state;

	using mptr = State* (State::*)();
	void Exec(mptr func) {
		State *new_state = (*current_state.*func)();
		if(new_state!=current_state) {
			delete current_state;
			current_state = new_state;
		}
	}
};

}
