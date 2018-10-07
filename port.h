/*
 * port.h
 *
 *  Created on: 2017?10?26?
 *      Author: dell
 */

#ifndef PORT_H_
#define PORT_H_
#include<string.h>
#include"quic_time.h"
#include"quic_type.h"
#include<memory>
#include<queue>
#include "actor.h"
struct Packet
{
	Packet();
	~Packet();
	Packet(const Packet &packet);
	std::string source;
	std::string destination;
	QuicTime timestamp;
	std::string content;
	QuicByteCount size;
};
namespace simulator
{

class UnconstrainedPortInterface
{
public:
	virtual ~UnconstrainedPortInterface(){}
	virtual void AcceptPacket(std::unique_ptr<struct Packet>packet)=0;
};
class ConstrainedPortInterface
{
public:
	virtual ~ConstrainedPortInterface(){}
	virtual void AcceptPacket(std::unique_ptr<struct Packet> packet)=0;
	virtual QuicTime::Delta TimeUntilAvailable()=0;

};
class EndPoint:public Actor
{
public:
	virtual UnconstrainedPortInterface *GetRxPort()=0;
	virtual void SetTxPort(ConstrainedPortInterface *port)=0;
protected:
	EndPoint(Simulator *simulator,std::string name);
};
}



#endif /* PORT_H_ */
