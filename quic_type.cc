#include "quic_type.h"
WriteResult::WriteResult() : status(WRITE_STATUS_ERROR), bytes_written(0) {}

WriteResult::WriteResult(WriteStatus status, int bytes_written_or_error_code)
    : status(status), bytes_written(bytes_written_or_error_code) {}
