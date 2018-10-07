/*
 * quic_type.h
 *
 *  Created on: 2017?10?26?
 *      Author: dell
 */
#ifndef QUIC_TYPE_H_
#define QUIC_TYPE_H_
#include <stdint.h>
#include<string>
#include<inttypes.h>//for PRId64
#ifdef _LP64
#define __PRIS_PREFIX "z"
#else
#define __PRIS_PREFIX
#endif
#define PRIdS __PRIS_PREFIX "d"
#define PRIxS __PRIS_PREFIX "x"
#define PRIuS __PRIS_PREFIX "u"
#define PRIXS __PRIS_PREFIX "X"
#define PRIoS __PRIS_PREFIX "o"
typedef  uint64_t QuicByteCount;
typedef uint64_t QuicPacketCount;

// Simple time constants.
const uint64_t kNumSecondsPerMinute = 60;
const uint64_t kNumSecondsPerHour = kNumSecondsPerMinute * 60;
const uint64_t kNumSecondsPerWeek = kNumSecondsPerHour * 24 * 7;
const uint64_t kNumMicrosPerMilli = 1000;
const uint64_t kNumMicrosPerSecond = 1000 * 1000;

// Default initial maximum size in bytes of a QUIC packet.
const QuicByteCount kDefaultMaxPacketSize = 1350;
// Default initial maximum size in bytes of a QUIC packet for servers.
const QuicByteCount kDefaultServerMaxPacketSize = 1000;
// The maximum packet size of any QUIC packet, based on ethernet's max size,
// minus the IP and UDP headers. IPv6 has a 40 byte header, UDP adds an
// additional 8 bytes.  This is a total overhead of 48 bytes.  Ethernet's
// max packet size is 1500 bytes,  1500 - 48 = 1452.
const QuicByteCount kMaxPacketSize = 1452;
// Default maximum packet size used in the Linux TCP implementation.
// Used in QUIC for congestion window computations in bytes.
const QuicByteCount kDefaultTCPMSS = 1460;

enum class Perspective{IS_SERVER,IS_CLIENT};

// TODO(wtc): see if WriteStatus can be replaced by QuicAsyncStatus.
enum WriteStatus {
  WRITE_STATUS_OK,
  WRITE_STATUS_BLOCKED,
  WRITE_STATUS_ERROR,
};

// A struct used to return the result of write calls including either the number
// of bytes written or the error code, depending upon the status.
struct  WriteResult {
  WriteResult(WriteStatus status, int bytes_written_or_error_code);
  WriteResult();

  WriteStatus status;
  union {
    int bytes_written;  // only valid when status is WRITE_STATUS_OK
    int error_code;     // only valid when status is WRITE_STATUS_ERROR
  };
};
#endif /* QUIC_TYPE_H_ */
