/*
 * simulator.cc
 *
 *  Created on: 2017?10?25?
 *      Author: dell
 */

#include"simulator.h"
namespace simulator
{

Simulator::Simulator():run_for_should_stop_(false),
enable_random_delays_(false),alarm_factory_(this,"default_alarm_manager")
{
    run_for_alarm_.reset(alarm_factory_.CreateAlarm(new RunForDelegate(&run_for_should_stop_)));
}
Simulator::~Simulator(){}
Simulator::Clock::Clock():now_(kStartTime){}
QuicTime Simulator::Clock::ApproximateNow()const{return now_;}
QuicTime Simulator::Clock::Now()const{return now_;}
QuicWallTime Simulator::Clock::WallNow()const
{
      return QuicWallTime::FromUNIXMicroseconds(
      (now_ - QuicTime::Zero()).ToMicroseconds());
}
Simulator::RunForDelegate::RunForDelegate(bool *run_for_should_stop):
 run_for_should_stop_(run_for_should_stop){}
 void Simulator::RunForDelegate::OnAlarm(){*run_for_should_stop_=true;}
void Simulator::AddActor(Actor*actor)
{
    auto empalce_time_result=schedule_time_.insert(std::make_pair(actor,QuicTime::Infinite()));
    auto empalce_name_result=actor_names_.insert(actor->name());
}
void Simulator::Schedule(Actor*actor,QuicTime new_time)
{
    auto schedule_time_it=schedule_time_.find(actor);
    if(schedule_time_it==schedule_time_.end())
    {
        log_error("schedule_time_it==schedule_time_.end()\n");
        exit(1);
    }
    QuicTime schedule_time=schedule_time_it->second;
    if(schedule_time<=new_time)
    {
        return ;
    }
    if(schedule_time!=QuicTime::Infinite())
    {
        Unschedule(actor);
    }
    schedule_time_it->second=new_time;
    schedule_.insert(std::make_pair(new_time,actor));
}
void Simulator::Unschedule(Actor *actor)
{
    auto schedule_time_it=schedule_time_.find(actor);
    if(schedule_time_it==schedule_time_.end())
    {
        log_error("schedule_time_it==schedule_time_.end()\n");
        exit(1);
    }
    QuicTime schedule_time=schedule_time_it->second;
    if(schedule_time==QuicTime::Infinite())
    {
        log_error("schedule_time==QuicTime::Infinite()\n");
    }
    auto range=schedule_.equal_range(schedule_time);
    {
        for(auto it=range.first;it!=range.second;it++)
        {
            if(it->second==actor)
            {
                schedule_.erase(it);
                schedule_time_it->second=QuicTime::Infinite();
                return ;
            }
        }
    }
}
void Simulator::set_random_generator(QuicRandom *random)
{
    random_generator_=random;
}
QuicRandom* Simulator::GetRandomGenerator()
{
    if(random_generator_==nullptr)
    {
        random_generator_=QuicRandom::GetInstance();
    }
    return random_generator_;
}
template<class Termination>
bool Simulator::RunUntil(Termination termination)
{
    bool predication_value=false;
    while(true)
    {
        predication_value=termination();
        if(predication_value||schedule_.empty())
        {
            break;
        }
        HandleNextScheduleActor();
    }
    return predication_value;
}
void Simulator::RunFor(QuicTime::Delta delta)
{
   run_for_should_stop_=false;
   QuicTime endtime=clock_.Now()+delta;
   run_for_alarm_->Set(endtime);
    bool simulation_result=RunUntil([this](){
        return run_for_should_stop_;
                    });
    if(clock_.Now()!=endtime)
    {
        log_error("clock_.Now()!=endtime");
    }
    if(simulation_result!=true)
    {
        log_error("simulation_result!=true");
    }

}
void Simulator::HandleNextScheduleActor()
{
    const auto current_event_it = schedule_.begin();
    QuicTime event_time = current_event_it->first;
    Actor* actor = current_event_it->second;
    log_debug("at time t %lld,schedule actor %s\n",event_time.ToDebuggingValue(),actor->name().c_str());
    Unschedule(actor);

    if(clock_.Now()>event_time)
    {
        log_error("clock_->Now()>Event_time\n");
        exit(1);
    }
    clock_.now_=event_time;
    actor->Act();
}
}


