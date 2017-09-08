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

	template<typename Context, typename Functor>
	void exec(Context &context, Functor functor)
	{
		if(current_state==end_state()) return;
		current_state->on_exit(context);

		(current_state->*functor)(context);

		if(current_state==end_state()) return;
		current_state->on_enter(context);
	}
};
