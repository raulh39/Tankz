#include "fsm.h"

void SelectingTankToFire::on_enter(FSM &fsm)
{
	fsm.HighlightSelectedTank();
}

void SelectingTankToFire::on_exit(FSM &fsm)
{
	fsm.UnhighlightSelectedTank();
}

State::state_type SelectingTankToFire::on_cycle(FSM &fsm) {
	fsm.IncSelected();
	return state<SelectingTankToFire>();
}

State::state_type SelectingTankToFire::on_select(FSM &fsm)
{
	fsm.SelectObjectivesGroup();
	return state<SelectingTarget>();
}

void SelectingTarget::on_enter(FSM &fsm)
{
	fsm.HighlightSelectedObjective();
}

void SelectingTarget::on_exit(FSM &fsm)
{
	fsm.UnhighlightSelectedObjective();
}

State::state_type SelectingTarget::on_cycle(FSM &fsm) {
	fsm.IncSelectedObjective();
	return state<SelectingTarget>();
}

State::state_type SelectingTarget::on_select(FSM &fsm)
{
	fsm.AssignDamage();
	if(fsm.MoreTanksToFire()) return state<SelectingTankToFire>();
	return end_state();
}
