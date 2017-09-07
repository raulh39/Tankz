#pragma once

class Game;

class State
{
public:
	virtual ~State() {}
	virtual void on_enter(Game &fsm) = 0;
	virtual void on_exit(Game &fsm) = 0;
	virtual void on_cycle(Game &fsm) = 0;
	virtual void on_select(Game &fsm) = 0;
};

class SelectingTankToFire: public State
{
public:
	virtual void on_enter(Game &fsm) override;
	virtual void on_exit(Game &fsm) override;
	virtual void on_cycle(Game &fsm) override;
	virtual void on_select(Game &fsm) override;
};

class SelectingTarget: public State
{
public:
	virtual void on_enter(Game &fsm) override;
	virtual void on_exit(Game &fsm) override;
	virtual void on_cycle(Game &fsm) override;
	virtual void on_select(Game &fsm) override;
};

