#include <iostream>
#include <functional>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>

class FSM;

class State
{
public:
	using state_type = std::type_index;
	virtual ~State() {}
	virtual void on_enter(FSM &fsm) = 0;
	virtual void on_exit(FSM &fsm) = 0;
	virtual state_type on_cycle(FSM &fsm) = 0;
	virtual state_type on_select(FSM &fsm) = 0;
	template <typename T>
	state_type state() { return typeid(T); }
};

//TODO: Instead of on_XXXX() make Event a class and then
//           template <Event> void on<Event>(Event&evt);
class FSM
{
public:
	FSM();
	void on_cycle();
	void on_select();

private:
	using tmap = std::unordered_map<State::state_type, State*>;
	tmap states;
	State::state_type state;

public: //TODO: maybe this have to go to a derived class

	//Functions used when transitioning states:
	void HighlightSelectedTank();
	void UnhighlightSelectedTank();
	void IncSelected();

	void SelectObjectivesGroup();
	void HighlightSelectedObjective();
	void UnhighlightSelectedObjective();
	void IncSelectedObjective();
	void AssignDamage();

	//Guards:
	bool MoreTanksToFireExecuted = false;
	bool MoreTanksToFire();
};

class SelectingTankToFire: public State
{
public:
	virtual void on_enter(FSM &fsm) override {}
	virtual void on_exit(FSM &fsm) override {}
	virtual state_type on_cycle(FSM &fsm) override;
	virtual state_type on_select(FSM &fsm) override;
};

class SelectingTarget: public State
{
public:
	virtual void on_enter(FSM &fsm) override {}
	virtual void on_exit(FSM &fsm) override {}
	virtual state_type on_cycle(FSM &fsm) override;
	virtual state_type on_select(FSM &fsm) override;
};


State::state_type SelectingTankToFire::on_cycle(FSM &fsm) {
	return state<SelectingTankToFire>();
}

State::state_type SelectingTankToFire::on_select(FSM &fsm)
{
	return state<SelectingTarget>();
}

State::state_type SelectingTarget::on_cycle(FSM &fsm) {
	return state<SelectingTarget>();
}

State::state_type SelectingTarget::on_select(FSM &fsm)
{
	return state<SelectingTankToFire>();
}

//TODO: repeated code
void FSM::on_cycle()
{
	states[state]->on_exit(*this);
	state = states[state]->on_cycle(*this);
	states[state]->on_enter(*this);
}

void FSM::on_select()
{
	states[state]->on_exit(*this);
	state = states[state]->on_select(*this);
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

int main()
{
	FSM game;
	std::cout << "\nPressinng Cycle()\n";
	game.on_cycle();
	std::cout << "\nPressinng Select()\n";
	game.on_select();
	std::cout << "\nPressinng Cycle()\n";
	game.on_cycle();
	std::cout << "\nPressinng Cycle()\n";
	game.on_cycle();
	std::cout << "\nPressinng Select()\n";
	game.on_select();
	std::cout << "\nPressinng Cycle()\n";
	game.on_cycle();
	std::cout << "\nPressinng Cycle()\n";
	game.on_cycle();
	std::cout << "\nPressinng Select()\n";
	game.on_select();
	std::cout << "\nPressinng Cycle()\n";
	game.on_cycle();
	std::cout << "\nPressinng Select()\n";
	game.on_select();
	std::cout << "\nPressinng Cycle()\n";
	game.on_cycle();
	std::cout << "\nPressinng Select()\n";
	game.on_select();
}

