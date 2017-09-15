#pragma once

#include <boost/statechart/event.hpp>
#include "TankzGameState.h"

namespace sc = boost::statechart;

struct EvCycle: sc::event<EvCycle>
{
	EvCycle(bool fwd): forward(fwd) {}
	bool forward;
};

struct EvEndGroup: sc::event<EvEndGroup>
{
};

struct EvEndPhase: sc::event<EvEndPhase>
{
	EvEndPhase(TankzPhase np):newPhase(np) {}
	TankzPhase newPhase;
};

struct EvEsc: sc::event<EvEsc>
{
};

struct EvMove: sc::event<EvMove>
{
	EvMove(float x, float y): valueX(x), valueY(y) {}
	float valueX;
	float valueY;
};

struct EvPan: sc::event<EvPan>
{
	EvPan(float x, float y): valueX(x), valueY(y) {}
	float valueX;
	float valueY;
};

struct EvSelect: sc::event<EvSelect>
{
};
