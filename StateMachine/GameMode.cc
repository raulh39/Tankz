#include "States.h"
#include <iostream>

void GameMode::AdjustArrowBase(const EvMove&)
{
	std::cout << "    AdjustArrowBase()\n";
}

void GameMode::AdjustArrowHead(const EvPan&)
{
	std::cout << "    AdjustArrowHead()\n";
}

void GameMode::AdjustTankPosition(const EvMove&)
{
	std::cout << "    AdjustTankPosition()\n";
}

void GameMode::AssignDamageAndMarkTankHasActed(const EvSelect&)
{
	std::cout << "    AssignDamageAndMarkTankHasActed()\n";
}

void GameMode::CalculateTankCommandActions(const EvSelect&)
{
	std::cout << "    CalculateTankCommandActions()\n";
}

void GameMode::ExecuteSelectedActionAndMarkTankHasActed(const EvSelect&)
{
	std::cout << "    ExecuteSelectedActionAndMarkTankHasActed()\n";
}

void GameMode::IncSelected(const EvCycle&)
{
	std::cout << "    IncSelected()\n";
}

void GameMode::IncSelectedAction(const EvCycle&)
{
	std::cout << "    IncSelectedAction()\n";
}

void GameMode::IncSelectedObjective(const EvCycle&)
{
	std::cout << "    IncSelectedObjective()\n";
}

void GameMode::MarkTankHasActed(const EvEsc&)
{
	std::cout << "    MarkTankHasActed()\n";
}

void GameMode::SelectObjectivesGroup(const EvSelect&)
{
	std::cout << "    SelectObjectivesGroup()\n";
}


void GameMode::CalculateNextGroup()
{
	std::cout << "    CalculateNextGroup()\n";
}

void GameMode::HighlightSelectedAction()
{
	std::cout << "    HighlightSelectedAction()\n";
}

void GameMode::HighlightSelectedObjective()
{
	std::cout << "    HighlightSelectedObjective()\n";
}

void GameMode::HighlightSelectedTank()
{
	std::cout << "    HighlightSelectedTank()\n";
}

void GameMode::PlaceTankOnArrowSide()
{
	std::cout << "    PlaceTankOnArrowSide()\n";
}

void GameMode::PositionArrowBase()
{
	std::cout << "    PositionArrowBase()\n";
}

void GameMode::SetTanksToNotActed()
{
	std::cout << "    SetTanksToNotActed()\n";
}

void GameMode::SpawnArrow()
{
	std::cout << "    SpawnArrow()\n";
}

void GameMode::DeleteArrowAndPlaceMovToken()
{
	std::cout << "    DeleteArrowAndPlaceMovToken()\n";
}

void GameMode::UnhighlightSelectedAction()
{
	std::cout << "    UnhighlightSelectedAction()\n";
}

void GameMode::UnhighlightSelectedObjective()
{
	std::cout << "    UnhighlightSelectedObjective()\n";
}

void GameMode::UnhighlightSelectedTank()
{
	std::cout << "    UnhighlightSelectedTank()\n";
}




//Guards used by states:
bool GameMode::ASideHasWon()
{
	aSideHasWon = !aSideHasWon;
	if(aSideHasWon) {
		std::cout << "    returning false from ASideHasWon\n";
		return false;
	}
	std::cout << "    returning true from ASideHasWon\n";
	return true;
}

bool GameMode::MoreTanksInGroup()
{
	moreTanksInGroup = !moreTanksInGroup;
	if(moreTanksInGroup) {
		std::cout << "    returning false from MoreTanksInGroup\n";
		return false;
	}
	std::cout << "    returning true from MoreTanksInGroup\n";
	return true;
}

bool GameMode::MoreTanksToAct()
{
	moreTanksToAct = !moreTanksToAct;
	if(moreTanksToAct) {
		std::cout << "    returning false from MoreTanksToAct\n";
		return false;
	}
	std::cout << "    returning true from MoreTanksToAct\n";
	return true;
}

bool GameMode::MoreMovesLeft()
{
	moreMovesLeft=!moreMovesLeft;
	if(moreMovesLeft) {
		std::cout << "    returning false from MoreMovesLeft()\n";
		return false;
	}
	std::cout << "    returning true from MoreMovesLeft()\n";
	return true;
}
