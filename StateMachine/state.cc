#include "game.h"

void SelectingTankToFire::on_enter(Game &game)
{
	game.HighlightSelectedTank();
}

void SelectingTankToFire::on_exit(Game &game)
{
	game.UnhighlightSelectedTank();
}

void SelectingTankToFire::on_cycle(Game &game) {
	game.IncSelected();
	game.change_state<SelectingTankToFire>();
}

void SelectingTankToFire::on_select(Game &game)
{
	game.SelectObjectivesGroup();
	game.change_state<SelectingTarget>();
}

void SelectingTarget::on_enter(Game &game)
{
	game.HighlightSelectedObjective();
}

void SelectingTarget::on_exit(Game &game)
{
	game.UnhighlightSelectedObjective();
}

void SelectingTarget::on_cycle(Game &game) {
	game.IncSelectedObjective();
	game.change_state<SelectingTarget>();
}

void SelectingTarget::on_select(Game &game)
{
	game.AssignDamage();
	if(game.MoreTanksToFire())
		game.change_state<SelectingTankToFire>();
	else
		game.to_end_state();
}
