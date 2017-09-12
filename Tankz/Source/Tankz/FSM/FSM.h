#pragma once

#include <tuple>

template<typename BaseSate, typename... DerivedStates>
class FSM
{
private:
	std::tuple<DerivedStates...> states;
	BaseSate *current_state;

	BaseSate *end_state() const {
		return nullptr;
	}

public:
	template<typename T>
	void change_state() {
		current_state = &(std::get<T>(states));
	}

	void to_end_state() {
		current_state = nullptr;
	}

	template<typename Functor, typename... Args>
	void exec(Functor functor, Args... args)
	{
		if(current_state==end_state()) return;
		current_state->on_exit(args...);

		(current_state->*functor)(args...);

		if(current_state==end_state()) return;
		current_state->on_enter(args...);
	}
};
