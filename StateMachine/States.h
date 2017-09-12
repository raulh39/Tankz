#pragma once

#include "Events.h"
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list.hpp>

namespace sc = boost::statechart;
namespace mpl = boost::mpl;

//States
//--------------
struct MOVINGState;
struct MovingGroupState;
struct MovingOneTankState;
struct SelectingTankToMoveState;
struct PlacingArrowState;
struct PlacingTankOnArrowState;

struct FIRINGState;
struct FiringGroupState;
struct SelectingTankToFireState;
struct SelectingTargetState;

struct COMMANDState;
struct CommandingGroupState;
struct SelectingTankToCommandState;
struct SelectActionForTankState;

//Machine
//-----------------
//                                       CRTP      Initial state
class GameMode: public sc::state_machine<GameMode, MOVINGState>
{
public:
	//Functions used by states in transtitions:
	void AdjustArrowBase(const EvMove&);
	void AdjustArrowHead(const EvPan&);
	void AdjustTankPosition(const EvMove&);
	void AssignDamageAndMarkTankHasActed(const EvSelect&);
	void CalculateTankCommandActions(const EvSelect&);
	void ExecuteSelectedActionAndMarkTankHasActed(const EvSelect&);
	void IncSelected(const EvCycle&);
	void IncSelectedAction(const EvCycle&);
	void IncSelectedObjective(const EvCycle&);
	void MarkTankHasActed(const EvEsc&);
	void SelectObjectivesGroup(const EvSelect&);

	//Functions used by states on entry and exit:
	void CalculateNextGroup();
	void HighlightSelectedAction();
	void HighlightSelectedObjective();
	void HighlightSelectedTank();
	void PlaceTankOnArrowSide();
	void PositionArrowBase();
	void SetTanksToNotActed();
	void SpawnArrow();
	void DeleteArrowAndPlaceMovToken();
	void UnhighlightSelectedAction();
	void UnhighlightSelectedObjective();
	void UnhighlightSelectedTank();

	//Guards used by states:
	bool ASideHasWon();
	bool MoreMovesLeft();
	bool MoreTanksInGroup();
	bool MoreTanksToAct();

private:
	bool aSideHasWon=true;
	bool moreMovesLeft=true;
	bool moreTanksInGroup=true;
	bool moreTanksToAct=true;
};

//--------------------------------------
//-----  MOVE
//--------------------------------------

//                                   CRTP         Outter state  Initial state
struct MOVINGState: public sc::state<MOVINGState, GameMode,     MovingGroupState>
{
	MOVINGState(my_context ctx):  my_base(ctx) {
		context<GameMode>().SetTanksToNotActed();
	}
	//Table of:
	//                Event ->     New state
	typedef mpl::list<
		sc::transition<EvEndPhase, FIRINGState>
	> reactions;
};

//                                        CRTP              Outter state  Initial state
struct MovingGroupState: public sc::state<MovingGroupState, MOVINGState,  SelectingTankToMoveState>
{
	MovingGroupState(my_context ctx): my_base(ctx) {
		context<GameMode>().CalculateNextGroup();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::custom_reaction<EvEndGroup>
	> reactions;

	sc::result react(const EvEndGroup&) {
		bool moreTanksToAct = context<GameMode>().MoreTanksToAct();
		if(moreTanksToAct)
			return transit<MovingGroupState>();
		post_event( EvEndPhase() ); 
		return discard_event();
	}
};

//                                                CRTP                      Outter state
struct SelectingTankToMoveState: public sc::state<SelectingTankToMoveState, MovingGroupState>
{
	SelectingTankToMoveState(my_context ctx): my_base(ctx) {
		context<GameMode>().HighlightSelectedTank();
	}
	~SelectingTankToMoveState() {
		context<GameMode>().UnhighlightSelectedTank();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::transition<EvCycle, SelectingTankToMoveState, GameMode, &GameMode::IncSelected>,
		sc::transition<EvSelect, MovingOneTankState>
	> reactions;
};

//                                          CRTP                Outter state       Initial state
struct MovingOneTankState: public sc::state<MovingOneTankState, MovingGroupState,  PlacingArrowState>
{
	MovingOneTankState(my_context ctx): my_base(ctx) {
		context<GameMode>().SpawnArrow();
	}
	~MovingOneTankState() {
		context<GameMode>().DeleteArrowAndPlaceMovToken();
	}
	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::custom_reaction<EvEsc>
	> reactions;

	sc::result react(const EvEsc& evEsc) {
		context<GameMode>().MarkTankHasActed(evEsc);
		bool moreTanksInGroup = context<GameMode>().MoreTanksInGroup();
		if(moreTanksInGroup) {
			context<GameMode>().IncSelected(EvCycle());
			return transit<SelectingTankToMoveState>();
		}
		post_event( EvEndGroup() ); 
		return discard_event();
	}

};

//                                         CRTP               Outter state
struct PlacingArrowState: public sc::state<PlacingArrowState, MovingOneTankState>
{
	PlacingArrowState(my_context ctx): my_base(ctx) {
		context<GameMode>().PositionArrowBase();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::transition<EvMove, PlacingArrowState, GameMode, &GameMode::AdjustArrowBase>,
		sc::transition<EvPan,  PlacingArrowState, GameMode, &GameMode::AdjustArrowHead>,
		sc::transition<EvSelect, PlacingTankOnArrowState>
	> reactions;
};

//                                               CRTP                     Outter state
struct PlacingTankOnArrowState: public sc::state<PlacingTankOnArrowState, MovingOneTankState>
{
	PlacingTankOnArrowState(my_context ctx): sc::state<PlacingTankOnArrowState, MovingOneTankState>(ctx) {
		context<GameMode>().PlaceTankOnArrowSide();
	}
	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::transition<EvMove, PlacingTankOnArrowState, GameMode, &GameMode::AdjustTankPosition>,
		sc::transition<EvPan,  PlacingTankOnArrowState, GameMode, &GameMode::AdjustArrowHead>,
		sc::custom_reaction<EvSelect>
	> reactions;

	sc::result react(const EvSelect&) {
		bool moreMovsLeft = context<GameMode>().MoreMovesLeft();
		if(moreMovsLeft)
			return transit<PlacingArrowState>();
		post_event( EvEsc() ); 
		return discard_event();
	}
};

//--------------------------------------
//-----  FIRING
//--------------------------------------
//                                   CRTP         Outter state  Initial state
struct FIRINGState: public sc::state<FIRINGState, GameMode,     FiringGroupState>
{
	FIRINGState(my_context ctx):  my_base(ctx) {
		context<GameMode>().SetTanksToNotActed();
	}
	//Table of:
	//                Event ->     New state
	typedef mpl::list<
		sc::transition<EvEndPhase, COMMANDState>
	> reactions;
};

//                                        CRTP              Outter state  Initial state
struct FiringGroupState: public sc::state<FiringGroupState, FIRINGState,  SelectingTankToFireState>
{
	FiringGroupState(my_context ctx): my_base(ctx) {
		context<GameMode>().CalculateNextGroup();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::custom_reaction<EvEndGroup>
	> reactions;

	sc::result react(const EvEndGroup&) {
		bool moreTanksToAct = context<GameMode>().MoreTanksToAct();
		if(moreTanksToAct)
			return transit<FiringGroupState>();
		post_event( EvEndPhase() ); 
		return discard_event();
	}
};

//                                                CRTP                      Outter state
struct SelectingTankToFireState: public sc::state<SelectingTankToFireState, FiringGroupState>
{
	SelectingTankToFireState(my_context ctx): my_base(ctx) {
		context<GameMode>().HighlightSelectedTank();
	}
	~SelectingTankToFireState() {
		context<GameMode>().UnhighlightSelectedTank();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::transition<EvCycle, SelectingTankToFireState, GameMode, &GameMode::IncSelected>,
		sc::transition<EvSelect, SelectingTargetState   , GameMode, &GameMode::SelectObjectivesGroup>
	> reactions;
};

//                                            CRTP                      Outter state
struct SelectingTargetState: public sc::state<SelectingTargetState, FiringGroupState>
{
	SelectingTargetState(my_context ctx): my_base(ctx) {
		context<GameMode>().HighlightSelectedObjective();
	}
	~SelectingTargetState() {
		context<GameMode>().UnhighlightSelectedObjective();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::transition<EvCycle, SelectingTargetState, GameMode, &GameMode::IncSelectedObjective>,
		sc::transition<EvEsc,   SelectingTankToFireState>,
		sc::custom_reaction<EvSelect>
	> reactions;

	sc::result react(const EvSelect&evSelect) {
		context<GameMode>().AssignDamageAndMarkTankHasActed(evSelect);
		bool moreTanksInGroup = context<GameMode>().MoreTanksInGroup();
		if(moreTanksInGroup)
			return transit<SelectingTankToFireState>();
		post_event( EvEndGroup() );
		return discard_event();
	}

};



//--------------------------------------
//-----  COMMAND
//--------------------------------------
//                                   CRTP           Outter state  Initial state
struct COMMANDState: public sc::state<COMMANDState, GameMode,     CommandingGroupState>
{
	COMMANDState(my_context ctx):  my_base(ctx) {
		context<GameMode>().SetTanksToNotActed();
	}
	//Table of:
	//                Event ->     New state
	typedef mpl::list<
		sc::custom_reaction<EvEndPhase>
	> reactions;

	sc::result react(const EvEndPhase&evEndPhase) {
		bool aSideHasWon = context<GameMode>().ASideHasWon();
		if(!aSideHasWon)
			return transit<MOVINGState>();
		return terminate();
	}
};

//                                            CRTP                  Outter state  Initial state
struct CommandingGroupState: public sc::state<CommandingGroupState, COMMANDState, SelectingTankToCommandState>
{
	CommandingGroupState(my_context ctx): my_base(ctx) {
		context<GameMode>().CalculateNextGroup();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::custom_reaction<EvEndGroup>
	> reactions;

	sc::result react(const EvEndGroup&) {
		bool moreTanksToAct = context<GameMode>().MoreTanksToAct();
		if(moreTanksToAct)
			return transit<CommandingGroupState>();
		post_event( EvEndPhase() ); 
		return discard_event();
	}
};

//                                                   CRTP                         Outter state
struct SelectingTankToCommandState: public sc::state<SelectingTankToCommandState, CommandingGroupState>
{
	SelectingTankToCommandState(my_context ctx): my_base(ctx) {
		context<GameMode>().HighlightSelectedTank();
	}
	~SelectingTankToCommandState() {
		context<GameMode>().UnhighlightSelectedTank();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::transition<EvCycle, SelectingTankToCommandState, GameMode, &GameMode::IncSelected>,
		sc::transition<EvSelect, SelectActionForTankState  , GameMode, &GameMode::CalculateTankCommandActions>
	> reactions;
};

//                                                CRTP                      Outter state
struct SelectActionForTankState: public sc::state<SelectActionForTankState, CommandingGroupState>
{
	SelectActionForTankState(my_context ctx): my_base(ctx) {
		context<GameMode>().HighlightSelectedAction();
	}
	~SelectActionForTankState() {
		context<GameMode>().UnhighlightSelectedAction();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::transition<EvCycle, SelectActionForTankState, GameMode, &GameMode::IncSelectedAction>,
		sc::custom_reaction<EvSelect>
	> reactions;

	sc::result react(const EvSelect&evSelect) {
		context<GameMode>().ExecuteSelectedActionAndMarkTankHasActed(evSelect);
		bool moreTanksInGroup = context<GameMode>().MoreTanksInGroup();
		if(moreTanksInGroup)
			return transit<SelectingTankToCommandState>();
		post_event( EvEndGroup() );
		return discard_event();
	}

};
