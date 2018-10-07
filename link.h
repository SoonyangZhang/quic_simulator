/*
 * link.h
 *
 *  Created on: 2017?10?26?
 *      Author: dell
 */

#ifndef LINK_H_
#define LINK_H_
#include"port.h"
#include"quic_bandwidth.h"
#include"quic_time.h"
#include"simulator.h"
namespace simulator
{

class OneWayLink :public Actor,public ConstrainedPortInterface
{
public:
OneWayLink(Simulator *simulator,std::string name,
		UnconstrainedPortInterface*sink,
		QuicBandwidth bandwidth,
		QuicTime::Delta propagation_delay);
~OneWayLink() override;

void AcceptPacket(std::unique_ptr<Packet> packet) override;
QuicTime::Delta TimeUntilAvailable() override;
void Act() override;

inline QuicBandwidth bandwidth() const { return bandwidth_; }
inline QuicTime::Delta propagation_delay() const {
  return propagation_delay_;
}

private:
struct QueuedPacket {
  std::unique_ptr<Packet> packet;
  QuicTime dequeue_time;
  QueuedPacket(std::unique_ptr<Packet> packet, QuicTime dequeue_time);
  ~QueuedPacket();
};

// Schedule the next packet to be egressed out of the link if there are
// packets on the link.
void ScheduleNextPacketDeparture();

// Get the value of a random delay imposed on each packet in order to avoid
// artifical synchronization artifacts during the simulation.
QuicTime::Delta GetRandomDelay(QuicTime::Delta transfer_time);

UnconstrainedPortInterface* sink_;
std::queue<QueuedPacket> packets_in_transit_;

const QuicBandwidth bandwidth_;
const QuicTime::Delta propagation_delay_;

QuicTime next_write_at_;

DISALLOW_COPY_AND_ASSIGN(OneWayLink);
};
class SymmetricLink
{
public:
    SymmetricLink(Simulator*simulator,std::string name,
                  UnconstrainedPortInterface*sink_a,
                  UnconstrainedPortInterface*sink_b,
                  QuicBandwidth bandwidth,
                  QuicTime::Delta propagation_delay
                  )
    :a_to_b_link_(simulator,QuicStringPrintf("%s A-to-B",name.c_str()),
                  sink_b,bandwidth,propagation_delay),
     b_to_a_link_(simulator,QuicStringPrintf("%s B-To-A",name.c_str()),
                  sink_b,bandwidth,propagation_delay){}
    SymmetricLink(EndPoint *endpoint_a,
                  EndPoint *endpoint_b,
                  Simulator *simulator,
                  QuicBandwidth bandwidth,
                  QuicTime::Delta propagation_delay
                  ):SymmetricLink(simulator,
                QuicStringPrintf("Link [%s]<->[%s]",endpoint_a->name().c_str(),endpoint_b->name().c_str()),
                endpoint_a->GetRxPort(),endpoint_b->GetRxPort(),
                bandwidth,propagation_delay)
    {
        endpoint_a->SetTxPort(&a_to_b_link_);
        endpoint_b->SetTxPort(&b_to_a_link_);//pointer
    }
private:
    OneWayLink a_to_b_link_;
    OneWayLink b_to_a_link_;
};
}

#endif /* LINK_H_ */
