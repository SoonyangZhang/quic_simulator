/*
 * link.cc
 *
 *  Created on: 2017?10?26?
 *      Author: dell
 */
#include"link.h"

namespace simulator
{
        // Parameters for random noise delay.
const uint64_t kMaxRandomDelayUs = 10;
OneWayLink::QueuedPacket::QueuedPacket(std::unique_ptr<Packet> packet,
                                    QuicTime dequeue_time
                        ):packet(std::move(packet)),dequeue_time(dequeue_time){}
OneWayLink::QueuedPacket::~QueuedPacket(){}
OneWayLink::OneWayLink(Simulator *simulator,std::string name,
		UnconstrainedPortInterface*sink,
		QuicBandwidth bandwidth,
		QuicTime::Delta propagation_delay):Actor(simulator,name),
		sink_(sink),
		bandwidth_(bandwidth),
		propagation_delay_(propagation_delay),
		next_write_at_(QuicTime::Zero()){}
OneWayLink::~OneWayLink(){}
void OneWayLink::AcceptPacket(std::unique_ptr<Packet>packet)
{
    if(!TimeUntilAvailable().IsZero())
    {
        log_error("link time is not zero\n");
        exit(1);
    }
    QuicTime::Delta transfer_time=bandwidth_.TransferTime(packet->size);
    next_write_at_=clock_->Now()+transfer_time;
    packets_in_transit_.emplace(std::move(packet),
                                next_write_at_+propagation_delay_+GetRandomDelay(transfer_time));
    ScheduleNextPacketDeparture();

}
QuicTime::Delta OneWayLink::TimeUntilAvailable()
{
    const QuicTime now=clock_->Now();
    if(next_write_at_<=now)
    {
        return QuicTime::Delta::Zero();
    }
    return next_write_at_-now;// consider the link processing ability
}
void OneWayLink::Act()
{
    if(packets_in_transit_.empty())//for check
    {
        log_error("link queue is empty");
        exit(1);
    }
    if(!(packets_in_transit_.front().dequeue_time>=clock_->Now()))
    {
        log_error("dequeue_time>=clock->Now() failed\n");
        exit(1);
    }
    log_debug("sink packet\n");
    sink_->AcceptPacket(std::move(packets_in_transit_.front().packet));
    packets_in_transit_.pop();
    ScheduleNextPacketDeparture();
}
void OneWayLink::ScheduleNextPacketDeparture()
{
    if(packets_in_transit_.empty())
    {
        return;
    }
    Schedule(packets_in_transit_.front().dequeue_time);
}
QuicTime::Delta OneWayLink::GetRandomDelay(QuicTime::Delta transfer_time)
{
    if(!(simulator_->enable_random_delays()))
    {
        return QuicTime::Delta::Zero();
    }
    QuicTime::Delta delta=QuicTime::Delta::FromMicroseconds(
        simulator_->GetRandomGenerator()->RandUint64()%(kMaxRandomDelayUs+1));
    return std::min(delta,0.5*transfer_time);
}
}



