#pragma once

#include <iostream>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

class Game;

class State
{
public:
	virtual ~State() {}
	virtual void on_enter(Game &fsm) {};
	virtual void on_exit(Game &fsm) {};
	virtual void on_cycle(Game &fsm) {};
	virtual void on_select(Game &fsm) {};
};
