#include "fsm.h"

FSM::FSM(): state(typeid(SelectingTankToFire))
{
	states[typeid(SelectingTankToFire)] = &selectingTankToFire;
	states[typeid(SelectingTarget)] = &selectingTarget;
}

void FSM::on_cycle()
{
	exec(&State::on_cycle);
}

void FSM::on_select()
{
	exec(&State::on_select);
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
