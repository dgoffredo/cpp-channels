#ifndef INCLUDED_CHAN_ERRORS_ERRORCODE
#define INCLUDED_CHAN_ERRORS_ERRORCODE

#include <chan/errors/noexcept.h>

namespace chan {

// Simulate C++11's `enum class` in C++98.
class ErrorCode {
  public:
    // The values are always negative (to distinguish them from non-error
    // return values of `select`).
    enum Value {
        OTHER                = -1,
        CREATE_PIPE          = -2,
        GET_FILE_FLAGS       = -3,
        SET_FILE_NONBLOCKING = -4,
        DRAIN_PIPE           = -5,
        RESTORE_FILE_FLAGS   = -6,
        MUTEX_INIT           = -7,
        MUTEX_LOCK           = -8,
        MUTEX_UNLOCK         = -9,
        CURRENT_TIME         = -10,
        POLL                 = -11,
        READ                 = -12,
        PROTOCOL_WRITE       = -13,
        PROTOCOL_READ        = -14,
        PROTOCOL_READ_EOF    = -15,
        TRANSFER             = -16,
        SELECT_UNWINDING     = -17,
        WRITE                = -18
    };

  private:
    Value value;

  public:
    ErrorCode(Value) CHAN_NOEXCEPT;
    operator Value() const CHAN_NOEXCEPT;

    const char* message() const CHAN_NOEXCEPT;
};

inline ErrorCode::ErrorCode(Value initialValue) CHAN_NOEXCEPT
: value(initialValue) {
}

inline ErrorCode::operator ErrorCode::Value() const CHAN_NOEXCEPT {
    return value;
}

}  // namespace chan

#endif
