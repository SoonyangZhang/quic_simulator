#include "quic_endpoint.h"
#include "simulator.h"
#include "quic_printf.h"
#include "quic_constants.h"
#include "ptr_impl.h"
namespace simulator
{
QuicEndPoint::Writer::Writer(QuicEndPoint *endpoint):
    endpoint_(endpoint){}
QuicEndPoint::Writer::~Writer(){}
WriteResult QuicEndPoint::Writer::WritePacket(const char* buffer,
                    size_t buf_len)
{
    std::string content=std::string(buffer,buf_len);
    std::unique_ptr<Packet> packet=make_unique<Packet>();
    packet->source=endpoint_->name();
    packet->destination=endpoint_->peer_name_;
    packet->content=content;
    packet->size=buf_len;
    log_debug("endpoint_send packet\n");
    endpoint_->nic_tx_queue_.AcceptPacket(std::move(packet));


}
QuicEndPoint::QuicEndPoint(Simulator*simulator,std::string name,
                 std::string peer_name,Perspective perspective):
                 EndPoint(simulator,name),writer_(this),
                 connection_(&writer_,simulator->GetAlarmFactory(),perspective),
                 nic_tx_queue_(simulator,QuicStringPrintf("%s queue",name.c_str()),kMaxPacketSize * kTxQueueSize)
{
    peer_name_=peer_name;
    bytes_to_transfered_=0;
    bytes_to_transfer_=0;
}
QuicEndPoint::~QuicEndPoint(){}
void QuicEndPoint::Act(){}
void QuicEndPoint::AcceptPacket(std::unique_ptr<struct Packet>packet)
{
    log_debug("endpoint accept packet\n");
    connection_.ProcessUdpPacket(std::move(packet),clock_->Now());
}
void QuicEndPoint::SetTxPort(ConstrainedPortInterface *port)
{
    nic_tx_queue_.set_tx_port(port);
}
void QuicEndPoint::OnPacketDequeued()
{
//TODO
}
void QuicEndPoint::AddBytesToTransfer(QuicByteCount bytes)
{
    if(bytes>0)
    {
        Schedule(clock_->Now());
    }
    bytes_to_transfer_+=bytes;
    WriteStreamData();
}
void QuicEndPoint::WriteStreamData()
{
//TODO  for now just for test;
 int len=20;
 char  *buffer=new char[len];
 buffer[0]=12;
 connection_.SendStreamData(buffer,len);
 delete buffer;
}




}
