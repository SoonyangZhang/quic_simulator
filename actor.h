/*
 * actor.h
 *
 *  Created on: 2017?10?24?
 *      Author: dell
 */

#ifndef ACTOR_H_
#define ACTOR_H_
#include<string>
#include"macros.h"
#include"log.h"
#include"quic_clock.h"
#include"quic_printf.h"
#include"quic_time.h"
namespace simulator
{
class Simulator;
class Actor
{
public:
	Actor(Simulator *simulator,std::string name);
	virtual ~Actor();
	virtual void Act()=0;

    inline std::string name()const {return name_;}
    inline Simulator* simulator()const{return simulator_;}
protected:
	void Schedule(QuicTime new_time);
	void Unschedule();
	Simulator *simulator_;
	std::string name_;
	const QuicClock *clock_;
  // Since the Actor object registers itself with a simulator using a pointer to
  // itself, do not allow it to be moved.
private:
  Actor(Actor&&) = delete;
  Actor& operator=(Actor&&) = delete;
DISALLOW_COPY_AND_ASSIGN(Actor);
};
}




#endif /* ACTOR_H_ */
