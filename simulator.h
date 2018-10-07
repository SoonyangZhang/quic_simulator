/*
 * simultor.h
 *
 *  The code is taken from google quic protocol for simulation
 *
 */

#ifndef SIMULATOR_H_
#define SIMULATOR_H_
#include<string>
#include<map>
#include "quic_time.h"
#include "quic_clock.h"
#include "connection_interface.h"
#include"quic_random.h"
#include "quic_alarm.h"
#include "alarm_factory.h"
#include "quic_containers.h"
#include "actor.h"
namespace simulator
{
class Simulator:public QuicConnectionHelperInterface
{
public:
Simulator();
void AddActor(Actor *actor);
void Schedule(Actor* actor,QuicTime new_time);
void Unschedule(Actor *actor);
~Simulator()override;
const QuicClock*GetClock() const override{return &clock_;}
QuicRandom* GetRandomGenerator()override;
void set_random_generator(QuicRandom *random);
bool enable_random_delays(){return enable_random_delays_;}
template<class Termination>
bool RunUntil(Termination termination);
void RunFor(QuicTime::Delta delta);
void HandleNextScheduleActor();
QuicAlarmFactory *GetAlarmFactory(){return &alarm_factory_;}
private:
class Clock:public QuicClock
{
   public:
    // Do not start at zero as certain code can treat zero as an invalid
    // timestamp.
    const QuicTime kStartTime =
        QuicTime::Zero() + QuicTime::Delta::FromMicroseconds(1);

    Clock();

    QuicTime ApproximateNow() const override;
    QuicTime Now() const override;
    QuicWallTime WallNow() const override;
    QuicTime now_;
};

  // The delegate used for RunFor().
  class RunForDelegate : public QuicAlarm::Delegate {
   public:
    explicit RunForDelegate(bool* run_for_should_stop);
    void OnAlarm() override;

   private:
    // Pointer to |run_for_should_stop_| in the parent simulator.
    bool* run_for_should_stop_;
  };

private:
    Clock clock_;
    QuicRandom* random_generator_;
      // Flag used to stop simulations ran via RunFor().
    bool run_for_should_stop_;
  // Indicates whether the simulator should add random delays on the links in
  // order to avoid synchronization issues.
    bool enable_random_delays_;
    AlarmFactory alarm_factory_;
    std::unique_ptr<QuicAlarm>run_for_alarm_;
    QuicUnorderedMap<Actor*,QuicTime> schedule_time_;
    std::unordered_set<std::string> actor_names_;
    std::multimap<QuicTime,Actor*>schedule_;
    DISALLOW_COPY_AND_ASSIGN(Simulator);
};

}




#endif /* SIMULATOR_H_ */
