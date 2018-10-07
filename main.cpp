/*That is the work created by zsy for multi-path video transmission simulation based on webrtc
 * data  2017/10/24
 * author 865678017@qq.com
 *
 */
#include <iostream>
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<quic_random.h>
#include"log.h"
#include"quic_time.h"
#include "simulator.h"
#include "queue.h"
#include "alarm_factory.h"
#include "link.h"
#include "switch.h"
#include "quic_endpoint.h"
#include "quic_type.h"
const QuicBandwidth kDefaultBandwidth =
    QuicBandwidth::FromKBitsPerSecond(10 * 1000);
const QuicTime::Delta kDefaultPropagationDelay =
    QuicTime::Delta::FromMilliseconds(20);
const QuicByteCount kDefaultBdp = kDefaultBandwidth * kDefaultPropagationDelay;
int main()
{
    simulator::Simulator simulator;
    QuicTime::Delta delta=QuicTime::Delta::FromMilliseconds(60);

    simulator::Switch switch_(&simulator,std::string("switch"),
                              8,2*kDefaultBdp);
    simulator::QuicEndPoint sender(&simulator,std::string("sender"),
                                   std::string("receiver"),Perspective::IS_CLIENT);
    simulator::QuicEndPoint receiver(&simulator,std::string("receiver"),
                                     std::string("sender"),Perspective::IS_SERVER);
    simulator::SymmetricLink sender_and_switch(&sender,switch_.port(1),&simulator,
                             kDefaultBandwidth,kDefaultPropagationDelay);
    simulator::SymmetricLink switch_and_receiver(switch_.port(2),&receiver,&simulator,
                             kDefaultBandwidth,kDefaultPropagationDelay);
    sender.AddBytesToTransfer(200);
    simulator.RunFor(delta);
    printf("run time %llu\n",simulator.GetClock()->Now().ToDebuggingValue());
}
