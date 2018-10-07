#ifndef __QUIC_CONNECTION_H__
#define __QUIC_CONNECTION_H__
#include "quic_packet_writer.h"
#include "quic_alarm_factory.h"
#include "quic_time.h"
#include "port.h"
class QuicConnection
{
public:
    QuicConnection(QuicPacketWriter *writer,QuicAlarmFactory *alarmfactory,Perspective perspective);
    ~QuicConnection();
void ProcessUdpPacket(std::unique_ptr<struct Packet>packet,QuicTime recvtime);
void SendStreamData(const char*buffer,int len);
private:
    QuicPacketWriter *writer_;
    QuicAlarmFactory *alarm_factory_;
    Perspective perspective_;

};
#endif // __QUIC_CONNECTION_H__
