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
//                                                   CRTP      Initial state
class GameModeStateMachine: public sc::state_machine<GameModeStateMachine, MOVINGState>
{
public:
	//Functions used by states in transtitions:
	virtual void AdjustArrowBase(const EvMove&)                            =0;
	virtual void AdjustArrowHead(const EvPan&)                             =0;
	virtual void AdjustTankPosition(const EvMove&)                         =0;
	virtual void AssignDamageAndMarkTankHasActed(const EvSelect&)          =0;
	virtual void CalculateTankCommandActions(const EvSelect&)              =0;
	virtual void ExecuteSelectedActionAndMarkTankHasActed(const EvSelect&) =0;
	virtual void IncSelected(const EvCycle&)                               =0;
	virtual void IncSelectedAction(const EvCycle&)                         =0;
	virtual void IncSelectedObjective(const EvCycle&)                      =0;
	virtual void MarkTankHasActed(const EvEsc&)                            =0;
	virtual void SelectObjectivesGroup(const EvSelect&)                    =0;

	//Functions used by states on entry and exit:
	virtual void CalculateNextGroup()                                      =0;
	virtual void HighlightSelectedAction()                                 =0;
	virtual void HighlightSelectedObjective()                              =0;
	virtual void HighlightSelectedTank()                                   =0;
	virtual void PlaceTankOnArrowSide()                                    =0;
	virtual void PositionArrowBase()                                       =0;
	virtual void SetTanksToNotActed()                                      =0;
	virtual void SpawnArrow()                                              =0;
	virtual void DeleteArrowAndPlaceMovToken()                             =0;
	virtual void UnhighlightSelectedAction()                               =0;
	virtual void UnhighlightSelectedObjective()                            =0;
	virtual void UnhighlightSelectedTank()                                 =0;

	//Guards used by states:
	virtual bool ASideHasWon()      =0;
	virtual bool MoreMovesLeft()    =0;
	virtual bool MoreTanksInGroup() =0;
	virtual bool MoreTanksToAct()   =0;
};

//--------------------------------------
//-----  MOVE
//--------------------------------------

//                                   CRTP         Outter state  Initial state
struct MOVINGState: public sc::state<MOVINGState, GameModeStateMachine,     MovingGroupState>
{
	MOVINGState(my_context ctx):  my_base(ctx) {
		context<GameModeStateMachine>().SetTanksToNotActed();
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
		context<GameModeStateMachine>().CalculateNextGroup();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::custom_reaction<EvEndGroup>
	> reactions;

	sc::result react(const EvEndGroup&) {
		bool moreTanksToAct = context<GameModeStateMachine>().MoreTanksToAct();
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
		context<GameModeStateMachine>().HighlightSelectedTank();
	}
	~SelectingTankToMoveState() {
		context<GameModeStateMachine>().UnhighlightSelectedTank();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::transition<EvCycle, SelectingTankToMoveState, GameModeStateMachine, &GameModeStateMachine::IncSelected>,
		sc::transition<EvSelect, MovingOneTankState>
	> reactions;
};

//                                          CRTP                Outter state       Initial state
struct MovingOneTankState: public sc::state<MovingOneTankState, MovingGroupState,  PlacingArrowState>
{
	MovingOneTankState(my_context ctx): my_base(ctx) {
		context<GameModeStateMachine>().SpawnArrow();
	}
	~MovingOneTankState() {
		context<GameModeStateMachine>().DeleteArrowAndPlaceMovToken();
	}
	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::custom_reaction<EvEsc>
	> reactions;

	sc::result react(const EvEsc& evEsc) {
		context<GameModeStateMachine>().MarkTankHasActed(evEsc);
		bool moreTanksInGroup = context<GameModeStateMachine>().MoreTanksInGroup();
		if(moreTanksInGroup) {
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
		context<GameModeStateMachine>().PositionArrowBase();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::transition<EvMove, PlacingArrowState, GameModeStateMachine, &GameModeStateMachine::AdjustArrowBase>,
		sc::transition<EvPan,  PlacingArrowState, GameModeStateMachine, &GameModeStateMachine::AdjustArrowHead>,
		sc::transition<EvSelect, PlacingTankOnArrowState>
	> reactions;
};

//                                               CRTP                     Outter state
struct PlacingTankOnArrowState: public sc::state<PlacingTankOnArrowState, MovingOneTankState>
{
	PlacingTankOnArrowState(my_context ctx): sc::state<PlacingTankOnArrowState, MovingOneTankState>(ctx) {
		context<GameModeStateMachine>().PlaceTankOnArrowSide();
	}
	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::transition<EvMove, PlacingTankOnArrowState, GameModeStateMachine, &GameModeStateMachine::AdjustTankPosition>,
		sc::transition<EvPan,  PlacingTankOnArrowState, GameModeStateMachine, &GameModeStateMachine::AdjustArrowHead>,
		sc::custom_reaction<EvSelect>
	> reactions;

	sc::result react(const EvSelect&) {
		bool moreMovsLeft = context<GameModeStateMachine>().MoreMovesLeft();
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
struct FIRINGState: public sc::state<FIRINGState, GameModeStateMachine,     FiringGroupState>
{
	FIRINGState(my_context ctx):  my_base(ctx) {
		context<GameModeStateMachine>().SetTanksToNotActed();
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
		context<GameModeStateMachine>().CalculateNextGroup();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::custom_reaction<EvEndGroup>
	> reactions;

	sc::result react(const EvEndGroup&) {
		bool moreTanksToAct = context<GameModeStateMachine>().MoreTanksToAct();
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
		context<GameModeStateMachine>().HighlightSelectedTank();
	}
	~SelectingTankToFireState() {
		context<GameModeStateMachine>().UnhighlightSelectedTank();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::transition<EvCycle, SelectingTankToFireState, GameModeStateMachine, &GameModeStateMachine::IncSelected>,
		sc::transition<EvSelect, SelectingTargetState   , GameModeStateMachine, &GameModeStateMachine::SelectObjectivesGroup>
	> reactions;
};

//                                            CRTP                      Outter state
struct SelectingTargetState: public sc::state<SelectingTargetState, FiringGroupState>
{
	SelectingTargetState(my_context ctx): my_base(ctx) {
		context<GameModeStateMachine>().HighlightSelectedObjective();
	}
	~SelectingTargetState() {
		context<GameModeStateMachine>().UnhighlightSelectedObjective();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::transition<EvCycle, SelectingTargetState, GameModeStateMachine, &GameModeStateMachine::IncSelectedObjective>,
		sc::transition<EvEsc,   SelectingTankToFireState>,
		sc::custom_reaction<EvSelect>
	> reactions;

	sc::result react(const EvSelect&evSelect) {
		context<GameModeStateMachine>().AssignDamageAndMarkTankHasActed(evSelect);
		bool moreTanksInGroup = context<GameModeStateMachine>().MoreTanksInGroup();
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
struct COMMANDState: public sc::state<COMMANDState, GameModeStateMachine,     CommandingGroupState>
{
	COMMANDState(my_context ctx):  my_base(ctx) {
		context<GameModeStateMachine>().SetTanksToNotActed();
	}
	//Table of:
	//                Event ->     New state
	typedef mpl::list<
		sc::custom_reaction<EvEndPhase>
	> reactions;

	sc::result react(const EvEndPhase&evEndPhase) {
		bool aSideHasWon = context<GameModeStateMachine>().ASideHasWon();
		if(!aSideHasWon)
			return transit<MOVINGState>();
		return terminate();
	}
};

//                                            CRTP                  Outter state  Initial state
struct CommandingGroupState: public sc::state<CommandingGroupState, COMMANDState, SelectingTankToCommandState>
{
	CommandingGroupState(my_context ctx): my_base(ctx) {
		context<GameModeStateMachine>().CalculateNextGroup();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::custom_reaction<EvEndGroup>
	> reactions;

	sc::result react(const EvEndGroup&) {
		bool moreTanksToAct = context<GameModeStateMachine>().MoreTanksToAct();
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
		context<GameModeStateMachine>().HighlightSelectedTank();
	}
	~SelectingTankToCommandState() {
		context<GameModeStateMachine>().UnhighlightSelectedTank();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::transition<EvCycle, SelectingTankToCommandState, GameModeStateMachine, &GameModeStateMachine::IncSelected>,
		sc::transition<EvSelect, SelectActionForTankState  , GameModeStateMachine, &GameModeStateMachine::CalculateTankCommandActions>
	> reactions;
};

//                                                CRTP                      Outter state
struct SelectActionForTankState: public sc::state<SelectActionForTankState, CommandingGroupState>
{
	SelectActionForTankState(my_context ctx): my_base(ctx) {
		context<GameModeStateMachine>().HighlightSelectedAction();
	}
	~SelectActionForTankState() {
		context<GameModeStateMachine>().UnhighlightSelectedAction();
	}

	//Table of:
	//                Event ->  New state          [Trigger function context,  Trigger function]
	typedef mpl::list<
		sc::transition<EvCycle, SelectActionForTankState, GameModeStateMachine, &GameModeStateMachine::IncSelectedAction>,
		sc::custom_reaction<EvSelect>
	> reactions;

	sc::result react(const EvSelect&evSelect) {
		context<GameModeStateMachine>().ExecuteSelectedActionAndMarkTankHasActed(evSelect);
		bool moreTanksInGroup = context<GameModeStateMachine>().MoreTanksInGroup();
		if(moreTanksInGroup)
			return transit<SelectingTankToCommandState>();
		post_event( EvEndGroup() );
		return discard_event();
	}

};
