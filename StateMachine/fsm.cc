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

FSM::FSM(): state(typeid(SelectingTankToFire))
{
	states[typeid(SelectingTankToFire)] = &selectingTankToFire;
	states[typeid(SelectingTarget)] = &selectingTarget;
}

//TODO: repeated code
void FSM::on_cycle()
{
	if(state==State::end_state()) return;
	states[state]->on_exit(*this);

	state = states[state]->on_cycle(*this);

	if(state==State::end_state()) return;
	states[state]->on_enter(*this);
}

void FSM::on_select()
{
	if(state==State::end_state()) return;
	states[state]->on_exit(*this);

	state = states[state]->on_select(*this);

	if(state==State::end_state()) return;
	states[state]->on_enter(*this);
}


void FSM::HighlightSelectedTank()
{
	std::cout << "Executing HighlightSelectedTank\n";
}

void FSM::UnhighlightSelectedTank()
{
	std::cout << "Executing UnhighlightSelectedTank\n";
}

void FSM::IncSelected()
{
	std::cout << "Executing IncSelected\n";
}


void FSM::SelectObjectivesGroup()
{
	std::cout << "Executing SelectObjectivesGroup\n";
}

void FSM::HighlightSelectedObjective()
{
	std::cout << "Executing HighlightSelectedObjective\n";
}

void FSM::UnhighlightSelectedObjective()
{
	std::cout << "Executing UnhighlightSelectedObjective\n";
}

void FSM::IncSelectedObjective()
{
	std::cout << "Executing IncSelectedObjective\n";
}

void FSM::AssignDamage()
{
	std::cout << "Executing AssignDamage\n";
}

bool FSM::MoreTanksToFire()
{
	if(!MoreTanksToFireExecuted) {
		MoreTanksToFireExecuted=true;
		std::cout << "Returning true from MoreTanksToFire\n";
		return true;
	}
	std::cout << "Returning false from MoreTanksToFire\n";
	return false;
}
