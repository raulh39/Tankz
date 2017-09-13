#pragma once

#include <boost/statechart/event.hpp>

namespace sc = boost::statechart;

struct EvCycle: sc::event<EvCycle>
{
};

struct EvEndGroup: sc::event<EvEndGroup>
{
};

struct EvEndPhase: sc::event<EvEndPhase>
{
};

struct EvEsc: sc::event<EvEsc>
{
};

struct EvMove: sc::event<EvMove>
{
};

struct EvPan: sc::event<EvPan>
{
};

struct EvSelect: sc::event<EvSelect>
{
};
