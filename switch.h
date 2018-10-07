/*
 * switch.h
 *
 *  Created on: 2017?10?26?
 *      Author: dell
 */

#ifndef SWITCH_H_
#define SWITCH_H_
#include"port.h"
#include<deque>
#include"queue.h"
#include"ptr_impl.h"
#include<unordered_map>
typedef size_t SwitchPortNumber;
namespace simulator
{
class Switch
{
public:
	Switch(Simulator *simulator,
			std::string name,
			SwitchPortNumber port_count,
			QuicByteCount capacity);
	  ~Switch();
inline EndPoint* port(SwitchPortNumber port_number)
{
	if(port_number<1)
	{
		log_error("port number shoud be large than 0");
		exit(1);
	}
	return &ports_[port_number-1];
}
inline const Queue* port_queue(SwitchPortNumber port_number) const {
  return ports_[port_number - 1].queue();
}
inline Queue* port_queue(SwitchPortNumber port_number) {
  return ports_[port_number - 1].queue();
}
private:
	class Port:public EndPoint,public UnconstrainedPortInterface
	{
	public:
		Port(Simulator *simulator,std::string name,Switch *parent,
				SwitchPortNumber port_number,
				QuicByteCount capacity);
	    Port(Port&&) = delete;
	    ~Port() override {}
	    // Accepts packet to be routed into the switch.
	     void AcceptPacket(std::unique_ptr<Packet> packet) override;
	     // Enqueue packet to be routed out of the switch.
	     void EnqueuePacket(std::unique_ptr<Packet> packet);

	     UnconstrainedPortInterface* GetRxPort() override;
	     void SetTxPort(ConstrainedPortInterface* port) override;

	     void Act() override;

	     inline bool connected() const { return connected_; }
	     inline const Queue* queue() const { return &queue_; }
	     inline Queue* queue() { return &queue_; }

	    private:
	     Switch* parent_;
	     SwitchPortNumber port_number_;
	     bool connected_;
	     Queue queue_;
	     DISALLOW_COPY_AND_ASSIGN(Port);
	};
	  // Sends the packet to the appropriate port, or to all ports if the
	  // appropriate port is not known.
	  void DispatchPacket(SwitchPortNumber port_number,
	                      std::unique_ptr<Packet> packet);

	  std::deque<Port> ports_;
	  std::unordered_map<std::string, Port*> switching_table_;

	  DISALLOW_COPY_AND_ASSIGN(Switch);
};
}




#endif /* SWITCH_H_ */
