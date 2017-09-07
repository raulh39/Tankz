#pragma once

#include "state.h"
#include <tuple>

//TODO: Instead of on_XXXX() make Event a class and then
//           template <Event> void on<Event>(Event&evt);


template<typename BaseSate, typename... DerivedStates>
class FSM
{
private:
	std::tuple<DerivedStates...> states;
	BaseSate *current_state;

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
protected:
	BaseSate *end_state() {
		return nullptr;
	}


};
