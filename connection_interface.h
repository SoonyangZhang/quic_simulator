/*
 * connection_interface.h
 *
 *  Created on: 2017?10?25?
 *      Author: dell
 */

#ifndef CONNECTION_INTERFACE_H_
#define CONNECTION_INTERFACE_H_
#include "quic_clock.h"
#include"quic_random.h"
class QuicConnectionHelperInterface {
 public:
  virtual ~QuicConnectionHelperInterface() {}

  // Returns a QuicClock to be used for all time related functions.
  virtual const QuicClock* GetClock() const = 0;

  // Returns a QuicRandom to be used for all random number related functions.
  virtual QuicRandom* GetRandomGenerator() = 0;

  // Returns a QuicBufferAllocator to be used for all stream frame buffers.
  //virtual QuicBufferAllocator* GetStreamFrameBufferAllocator() = 0;

  // Returns a QuicBufferAllocator to be used for stream send buffers.
  //virtual QuicBufferAllocator* GetStreamSendBufferAllocator() = 0;
};



#endif /* CONNECTION_INTERFACE_H_ */
