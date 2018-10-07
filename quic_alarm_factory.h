#ifndef NET_QUIC_CORE_QUIC_ALARM_FACTORY_H_
#define NET_QUIC_CORE_QUIC_ALARM_FACTORY_H_
#include "quic_alarm.h"
// Creates platform-specific alarms used throughout QUIC.
class QUIC_EXPORT_PRIVATE QuicAlarmFactory {
 public:
  virtual ~QuicAlarmFactory() {}

  // Creates a new platform-specific alarm which will be configured to notify
  // |delegate| when the alarm fires. Returns an alarm allocated on the heap.
  // Caller takes ownership of the new alarm, which will not yet be "set" to
  // fire.
  virtual QuicAlarm* CreateAlarm(QuicAlarm::Delegate* delegate) = 0;

};
#endif
