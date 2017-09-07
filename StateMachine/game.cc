#include "game.h"
#include <iostream>

void Game::on_cycle()
{
	fsm.exec(*this, &State::on_cycle);
}

void Game::on_select()
{
	fsm.exec(*this, &State::on_select);
}

void Game::HighlightSelectedTank()
{
	std::cout << "Executing HighlightSelectedTank\n";
}

void Game::UnhighlightSelectedTank()
{
	std::cout << "Executing UnhighlightSelectedTank\n";
}

void Game::IncSelected()
{
	std::cout << "Executing IncSelected\n";
}


void Game::SelectObjectivesGroup()
{
	std::cout << "Executing SelectObjectivesGroup\n";
}

void Game::HighlightSelectedObjective()
{
	std::cout << "Executing HighlightSelectedObjective\n";
}

void Game::UnhighlightSelectedObjective()
{
	std::cout << "Executing UnhighlightSelectedObjective\n";
}

void Game::IncSelectedObjective()
{
	std::cout << "Executing IncSelectedObjective\n";
}

void Game::AssignDamage()
{
	std::cout << "Executing AssignDamage\n";
}

bool Game::MoreTanksToFire()
{
	if(!MoreTanksToFireExecuted) {
		MoreTanksToFireExecuted=true;
		std::cout << "Returning true from MoreTanksToFire\n";
		return true;
	}
	std::cout << "Returning false from MoreTanksToFire\n";
	return false;
}

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
