/*
 * queue.cc
 *
 *  Created on: 2017?10?26?
 *      Author: dell
 */

#include"queue.h"
namespace simulator
{
Queue::ListenerInterface::~ListenerInterface(){}
Queue::EnqueuedPacket::EnqueuedPacket(std::unique_ptr<Packet>packet):packet(std::move(packet)){}
Queue::EnqueuedPacket::~EnqueuedPacket()=default;
Queue::Queue(Simulator*simulator,std::string name,
                 QuicByteCount capacity):Actor(simulator,name),
                 capacity_(capacity),bytes_queued_(0),listener_(nullptr)
{

}
Queue::~Queue(){}
void Queue::set_tx_port(ConstrainedPortInterface*port)
{
    tx_port_=port;
}
void Queue::AcceptPacket(std::unique_ptr<Packet>packet)
{
    if(packet->size+bytes_queued_>capacity_)
    {
        log_warn("queue %s is out of buffer,packet %s to %s is lost\n",
                 name().c_str(),packet->source.c_str(),packet->destination.c_str());
        return ;
    }
    log_debug("queue accept packet %s ->%s",packet->source.c_str(),packet->destination.c_str());
    bytes_queued_+=packet->size;
    queue_.emplace(std::move(packet));

    ScheduleNextPacketDequeue();
}
void Queue::Act()
{
    QuicTime link_process_delay=clock_->Now()+tx_port_->TimeUntilAvailable();
    log_debug("link process delay %llu",link_process_delay.ToDebuggingValue());
    if(tx_port_->TimeUntilAvailable().IsZero())
    {
        if(!(bytes_queued_>=queue_.front().packet->size))
        {
            log_error("bytes_queued_>front_packet_size\n");
        }
        bytes_queued_-=queue_.front().packet->size;
        log_debug("send packet at time%llu\n",clock_->Now().ToDebuggingValue());
        tx_port_->AcceptPacket(std::move(queue_.front().packet));
        queue_.pop();
        if(listener_!=nullptr)
        {
            listener_->OnPacketDequeued();
        }
    }
    ScheduleNextPacketDequeue();
}
void Queue::ScheduleNextPacketDequeue()
{
    if(queue_.empty())
    {
        if(bytes_queued_>0)
            log_warn("queue is empty but the queue counter is not 0\n");
        return ;
    }
QuicTime register_time=clock_->Now()+tx_port_->TimeUntilAvailable();
log_debug("queue clock now %llu\n",clock_->Now().ToDebuggingValue());
log_debug(" register at time t %lld\n",register_time.ToDebuggingValue());
Schedule(clock_->Now()+tx_port_->TimeUntilAvailable());
}
}


