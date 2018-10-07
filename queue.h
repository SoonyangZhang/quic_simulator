/*
 * queue.h
 *
 *  Created on: 2017?10?26?
 *      Author: dell
 */

#ifndef QUEUE_H_
#define QUEUE_H_
#include "actor.h"
#include"port.h"
namespace simulator
{
class Queue:public Actor,public UnconstrainedPortInterface
{
public:
 class ListenerInterface {
  public:
   virtual ~ListenerInterface();

   // Called whenever a packet is removed from the queue.
   virtual void OnPacketDequeued() = 0;
 };
   // In order to implement packet aggregation, each packet is tagged with a
  // bundle number.  The queue keeps a bundle counter, and whenever a bundle is
  // ready, it increments the number of the current bundle.  Only the packets
  // outside of the current bundle are allowed to leave the queue.
public:
	Queue(Simulator *simualtor,std::string name,QuicByteCount capacity);
	  ~Queue() override;

	  void set_tx_port(ConstrainedPortInterface* port);

	  void AcceptPacket(std::unique_ptr<Packet> packet) override;

	  void Act() override;

	  inline QuicByteCount capacity() const { return capacity_; }
	  inline QuicByteCount bytes_queued() const { return bytes_queued_; }
	  inline QuicPacketCount packets_queued() const { return queue_.size(); }

	  inline void set_listener_interface(ListenerInterface* listener) {
	    listener_ = listener;
	  }
 void ScheduleNextPacketDequeue();
private:
  struct EnqueuedPacket {
    EnqueuedPacket(std::unique_ptr<Packet> packet);
    ~EnqueuedPacket();
    std::unique_ptr<Packet> packet;
  };
private:
	  std::queue<EnqueuedPacket>queue_;
	  QuicByteCount capacity_;
	  QuicByteCount bytes_queued_;
	  ListenerInterface *listener_;
	  ConstrainedPortInterface *tx_port_;
	  DISALLOW_COPY_AND_ASSIGN(Queue);

};
}




#endif /* QUEUE_H_ */
