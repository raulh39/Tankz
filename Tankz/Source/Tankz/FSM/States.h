#pragma once

#include "FSM/FSM.h"

class ATankzGameModeBase;
class MoveState;
class MovingOneTank;


class State
{
public:
	virtual ~State() {}
	virtual void on_enter(ATankzGameModeBase &fsm) {};
	virtual void on_exit(ATankzGameModeBase &fsm) {};
	virtual void on_cycle(ATankzGameModeBase &fsm) {};
	virtual void on_select(ATankzGameModeBase &fsm) {};
};

class MoveStatesBase
{
public:
	virtual ~MoveStatesBase() {}
	virtual void on_enter(MoveState &fsm, ATankzGameModeBase &game) {};
	virtual void on_exit(MoveState &fsm, ATankzGameModeBase &game) {};
	virtual void on_cycle(MoveState &fsm, ATankzGameModeBase &game) {};
	virtual void on_select(MoveState &fsm, ATankzGameModeBase &game) {};
};

class SelectingTankToMove: public MoveStatesBase
{
public:
	virtual void on_enter(MoveState &fsm, ATankzGameModeBase &game) override;
	virtual void on_exit(MoveState &fsm, ATankzGameModeBase &game) override;
	virtual void on_cycle(MoveState &fsm, ATankzGameModeBase &game) override;
	virtual void on_select(MoveState &fsm, ATankzGameModeBase &game) override;
};

class PlacingArrow: public State
{
public:
	virtual void on_enter(ATankzGameModeBase &fsm) override;
	virtual void on_exit(ATankzGameModeBase &fsm) override;
	virtual void on_cycle(ATankzGameModeBase &fsm) override;
	virtual void on_select(ATankzGameModeBase &fsm) override;
};

class PlacingTankOnArrow: public State
{
public:
	virtual void on_enter(ATankzGameModeBase &fsm) override;
	virtual void on_exit(ATankzGameModeBase &fsm) override;
	virtual void on_cycle(ATankzGameModeBase &fsm) override;
	virtual void on_select(ATankzGameModeBase &fsm) override;
};

class MovingOneTank: public MoveStatesBase
{
public:
	virtual void on_enter(MoveState &fsm, ATankzGameModeBase &game) override;
	virtual void on_exit(MoveState &fsm, ATankzGameModeBase &game) override;
	virtual void on_cycle(MoveState &fsm, ATankzGameModeBase &game) override;
	virtual void on_select(MoveState &fsm, ATankzGameModeBase &game) override;
private:
	FSM<State, PlacingArrow, PlacingTankOnArrow> fsm;
};

class MoveState: public State
{
public:
	virtual void on_enter(ATankzGameModeBase &fsm) override;
	virtual void on_cycle(ATankzGameModeBase &fsm) override;
	virtual void on_select(ATankzGameModeBase &fsm) override;

	template<class T>
	void change_state() {
		fsm.change_state<T>();
	}

	void to_end_state() {
		fsm.to_end_state();
	}

private:
	FSM<MoveStatesBase, SelectingTankToMove, MovingOneTank> fsm;
};


/*
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
*/
