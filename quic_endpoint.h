#ifndef __QUIC_EDNPOINT_H__
#define __QUIC_EDNPOINT_H__
#include "port.h"
#include "queue.h"
#include "quic_packet_writer.h"
#include "quic_connection.h"
namespace simulator
{
class Simulator;
// Size of the TX queue used by the kernel/NIC.  1000 is the Linux
// kernel default.
const QuicByteCount kTxQueueSize = 1000;
class QuicEndPoint:public EndPoint,UnconstrainedPortInterface,
Queue::ListenerInterface
{
public:
    QuicEndPoint(Simulator*simulator,std::string name,
                 std::string peer_name,Perspective perspective);
    ~QuicEndPoint() override;
    void Act() override;
    void AcceptPacket(std::unique_ptr<struct Packet>packet) override;
    UnconstrainedPortInterface *GetRxPort() override{ return this;}
    void SetTxPort(ConstrainedPortInterface *port) override;
    void OnPacketDequeued()override;
    void AddBytesToTransfer(QuicByteCount bytes);
    void WriteStreamData();
private:
class Writer: public QuicPacketWriter
{
public:
    Writer(QuicEndPoint *endpoint);
    ~Writer() override;
    WriteResult WritePacket(const char* buffer,
                    size_t buf_len)override;
private:
    QuicEndPoint *endpoint_;
};
private:
    QuicByteCount bytes_to_transfer_;
    QuicByteCount bytes_to_transfered_;
    Writer writer_;
    QuicConnection connection_;
    Queue nic_tx_queue_;
    std::string peer_name_;
};
}
#endif // __QUIC_EDNPOINT_H__
