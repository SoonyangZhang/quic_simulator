/*
 * switch.cc
 *
 *  Created on: 2017?10?26?
 *      Author: dell
 */

#include"switch.h"
#include<cinttypes>//for PRId64
namespace simulator
{
Switch::Port::Port(Simulator *simulator,std::string name,Switch *parent,
				SwitchPortNumber port_number,
				QuicByteCount capacity):EndPoint(simulator,name),
				parent_(parent),port_number_(port_number),connected_(false),
				queue_(simulator,QuicStringPrintf("%s queue",name.c_str()),capacity)
				{

				}
void Switch::Port::AcceptPacket(std::unique_ptr<Packet>packet)
{
    parent_->DispatchPacket(port_number_,std::move(packet));
}
void Switch::Port::EnqueuePacket(std::unique_ptr<Packet>packet)
{
    log_debug("enqueue packet to port %d\n",port_number_);
    queue_.AcceptPacket(std::move(packet));// the port has queue,queue should consider link processing capacity
}
void Switch::Port::Act(){}//Port in switch has no need to schedule in simulator
UnconstrainedPortInterface* Switch::Port::GetRxPort(){return this;}
void Switch::Port::SetTxPort(ConstrainedPortInterface*port)
{
    queue_.set_tx_port(port);
    connected_=true;
}
Switch::Switch(Simulator* simulator,
        std::string name,
        SwitchPortNumber port_count,
        QuicByteCount queue_capacity
		)
{
	for(size_t port_number=1;port_number<=port_count;port_number++)
	{
        ports_.emplace_back(simulator,QuicStringPrintf("%s (port %" PRIuS ")",
                                                    name.c_str(), port_number),
                            this,port_number,queue_capacity);
	}
}
Switch::~Switch(){}
void Switch::DispatchPacket(SwitchPortNumber port_number,std::unique_ptr<Packet>packet)
{
   Port*port=&ports_[port_number-1];
   log_debug("switch dispatch packet\n");
   const auto source_mapping_it=switching_table_.find(packet->source);
   if(source_mapping_it==switching_table_.end())
   {
       switching_table_.insert(std::make_pair(packet->source,port));
   }
   const auto destination_mapping_it=switching_table_.find(packet->destination);
   if(destination_mapping_it!=switching_table_.end())
   {
       destination_mapping_it->second->EnqueuePacket(std::move(packet));
       return ;
   }
   //else not find the destination port,dispatch packet to all port except the source
    for(int i=0;i<ports_.size();i++)
    {
        Port *egress_port=&ports_[i];

        if(egress_port!=port&&egress_port->connected())//should not send packet to port has no receiver link
        {
            egress_port->EnqueuePacket(make_unique<Packet>(*packet));
        }
    }
    return ;

}

}


