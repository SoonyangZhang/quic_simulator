/*
 * quic_clock.h
 *
 *  Created on: 2017?10?24?
 *      Author: dell
 */

#ifndef QUIC_CLOCK_H_
#define QUIC_CLOCK_H_
#include "quic_export.h"
#include"quic_time.h"
// Interface for retreiving the current time.
class QUIC_EXPORT_PRIVATE QuicClock {
 public:
  QuicClock();
  virtual ~QuicClock();

  QuicClock(const QuicClock&) = delete;
  QuicClock& operator=(const QuicClock&) = delete;

  // Returns the approximate current time as a QuicTime object.
  virtual QuicTime ApproximateNow() const = 0;

  // Returns the current time as a QuicTime object.
  // Note: this use significant resources please use only if needed.
  virtual QuicTime Now() const = 0;

  // WallNow returns the current wall-time - a time that is consistent across
  // different clocks.
  virtual QuicWallTime WallNow() const = 0;

  // Converts |walltime| to a QuicTime relative to this clock's epoch.
  virtual QuicTime ConvertWallTimeToQuicTime(
      const QuicWallTime& walltime) const;

 protected:
  // Creates a new QuicTime using |time_us| as the internal value.
  QuicTime CreateTimeFromMicroseconds(uint64_t time_us) const {
    return QuicTime(time_us);
  }
};


#endif /* QUIC_CLOCK_H_ */
