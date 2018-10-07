/*
 * actor.cc
 *
 *  Created on: 2017?10?26?
 *      Author: dell
 */

#include"actor.h"
#include"simulator.h"
namespace simulator
{
Actor::Actor(Simulator*simulator,std::string name):
    simulator_(simulator),name_(name),clock_(simulator->GetClock())
{
    simulator->AddActor(this);
}
Actor::~Actor()
{

}
void Actor::Schedule(QuicTime new_time)
{
    simulator_->Schedule(this,new_time);
}
void Actor::Unschedule()
{
    simulator_->Unschedule(this);
}
}


