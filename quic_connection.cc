#include "quic_connection.h"
QuicConnection::QuicConnection(QuicPacketWriter *writer,
                               QuicAlarmFactory *alarmfactory,
                               Perspective perspective):
        writer_(writer),alarm_factory_(alarmfactory),perspective_(perspective){}
QuicConnection::~QuicConnection(){}
void QuicConnection::ProcessUdpPacket(std::unique_ptr<struct Packet>packet,QuicTime recvtime)
{
    std::string content=packet->content;
    const char *data=content.data();
    log_debug("receive data from peer%d\n",data[0]);

}
void QuicConnection::SendStreamData(const char*buffer,int len)
{
    writer_->WritePacket(buffer,len);
}
