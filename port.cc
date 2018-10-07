/*
 * port.cc
 *
 *  Created on: 2017?10?26?
 *      Author: dell
 */

#include"port.h"
Packet::Packet():source(),destination(),timestamp(QuicTime::Zero()),size(0){}
Packet::Packet(const Packet &packet)=default;
Packet::~Packet(){}
namespace simulator
{
EndPoint::EndPoint(Simulator *simulator,std::string name):Actor(simulator,name){}
}


