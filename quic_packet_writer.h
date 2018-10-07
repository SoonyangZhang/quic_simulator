#ifndef __QUIC_PACKET_WRITER_H__
#define __QUIC_PACKET_WRITER_H__
#include "quic_type.h"
class QuicPacketWriter
{
 public:
  virtual ~QuicPacketWriter() {}

  // Sends the packet out to the peer, with some optional per-packet options.
  // If the write succeeded, the result's status is WRITE_STATUS_OK and
  // bytes_written is populated. If the write failed, the result's status is
  // WRITE_STATUS_BLOCKED or WRITE_STATUS_ERROR and error_code is populated.
  // Options must be either null, or created for the particular QuicPacketWriter
  // implementation. Options may be ignored, depending on the implementation.
  virtual WriteResult WritePacket(const char* buffer,
                                  size_t buf_len) = 0;

  // Returns true if the writer buffers and subsequently rewrites data
  // when an attempt to write results in the underlying socket becoming
  // write blocked.
 // virtual bool IsWriteBlockedDataBuffered() const = 0;

  // Returns true if the network socket is not writable.
  //virtual bool IsWriteBlocked() const = 0;

  // Records that the socket has become writable, for example when an EPOLLOUT
  // is received or an asynchronous write completes.
  //virtual void SetWritable() = 0;

  // Returns the maximum size of the packet which can be written using this
  // writer for the supplied peer address.  This size may actually exceed the
  // size of a valid QUIC packet.
 // virtual QuicByteCount GetMaxPacketSize(
  //    const QuicSocketAddress& peer_address) const = 0;
};
#endif // __QUIC_PACKET_WRITER_H__
