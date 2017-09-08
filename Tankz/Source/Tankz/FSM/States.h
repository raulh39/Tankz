#pragma once

class ATankzGameModeBase;

class State
{
public:
	virtual ~State() {}
	virtual void on_enter(ATankzGameModeBase &fsm) = 0;
	virtual void on_exit(ATankzGameModeBase &fsm) = 0;
	virtual void on_cycle(ATankzGameModeBase &fsm) = 0;
	virtual void on_select(ATankzGameModeBase &fsm) = 0;
};

class SelectingTankToFire: public State
{
public:
	virtual void on_enter(ATankzGameModeBase &fsm) override;
	virtual void on_exit(ATankzGameModeBase &fsm) override;
	virtual void on_cycle(ATankzGameModeBase &fsm) override;
	virtual void on_select(ATankzGameModeBase &fsm) override;
};

class SelectingTarget: public State
{
public:
	virtual void on_enter(ATankzGameModeBase &fsm) override;
	virtual void on_exit(ATankzGameModeBase &fsm) override;
	virtual void on_cycle(ATankzGameModeBase &fsm) override;
	virtual void on_select(ATankzGameModeBase &fsm) override;
};
