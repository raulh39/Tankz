#include "game.h"

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
