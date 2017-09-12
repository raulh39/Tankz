#include "FSM/States.h"
#include "TankzGameModeBase.h"


void MoveState::on_enter(ATankzGameModeBase &game)
{
	game.ResetAllTanksAndSelectFirstGroupToAct();
}

void MoveState::on_cycle(ATankzGameModeBase &game)
{
	fsm.exec(&State::on_cycle, *this, game);
}

void MoveState::on_select(ATankzGameModeBase &game)
{
	fsm.exec(&State::on_select, *this, game);
}



/*
void SelectingTankToFire::on_enter(ATankzGameModeBase &game)
{
	game.HighlightSelectedTank();
}

void SelectingTankToFire::on_exit(ATankzGameModeBase &game)
{
	game.UnhighlightSelectedTank();
}

void SelectingTankToFire::on_cycle(ATankzGameModeBase &game) {
	game.IncSelected();
	game.change_state<SelectingTankToFire>();
}

void SelectingTankToFire::on_select(ATankzGameModeBase &game)
{
	game.SelectObjectivesGroup();
	game.change_state<SelectingTarget>();
}

void SelectingTarget::on_enter(ATankzGameModeBase &game)
{
	game.HighlightSelectedObjective();
}

void SelectingTarget::on_exit(ATankzGameModeBase &game)
{
	game.UnhighlightSelectedObjective();
}

void SelectingTarget::on_cycle(ATankzGameModeBase &game) {
	game.IncSelectedObjective();
	game.change_state<SelectingTarget>();
}

void SelectingTarget::on_select(ATankzGameModeBase &game)
{
	game.AssignDamage();
	if(game.MoreTanksToFire())
		game.change_state<SelectingTankToFire>();
	else
		game.to_end_state();
}
*/
