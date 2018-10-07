#include "quic_clock.h"
QuicClock::QuicClock() {}

QuicClock::~QuicClock() {}

QuicTime QuicClock::ConvertWallTimeToQuicTime(
    const QuicWallTime& walltime) const {
  //     ..........................
  //     |            |           |
  // unix epoch   |walltime|   WallNow()
  //     ..........................
  //            |     |           |
  //     clock epoch  |         Now()
  //               result
  //
  // result = Now() - (WallNow() - walltime)
  return Now() - QuicTime::Delta::FromMicroseconds(
                     WallNow()
                         .Subtract(QuicTime::Delta::FromMicroseconds(
                             walltime.ToUNIXMicroseconds()))
                         .ToUNIXMicroseconds());
}

