#include <chan/time/duration.h>

namespace chan {

long Duration::carefulDivide(Duration left, Duration right) {
    // This is a little tricky.  I break it into cases based on whether
    // `spec.seconds` is zero in either or both of `left` and `right`.
    const std::time_t leftSec  = left.spec.seconds,
                      rightSec = right.spec.seconds;
    const long leftNano        = left.spec.nanoseconds,
               rightNano       = right.spec.nanoseconds;

    if (leftSec == 0 && rightSec == 0) {
        // No seconds, so just divide the nanoseconds.
        return leftNano / rightNano;
    }
    else if (leftSec == 0) {
        // The numerator (left) has no seconds, but the divisor (right) does,
        // so we necessarily have smaller divided by bigger, which is zero.
        return 0;
    }
    else if (rightSec == 0) {
        // The numerator (left) has seconds, but the divisor (right) does not.
        // In order to reduce the likelihood of overflow on 32-bit systems, we
        // do some math here.
        //
        // Let `a` be `leftSec`, the number of seconds in the numerator.
        // Let `b` be `leftNano`, the number of nanoseconds in the numerator.
        // Let `d` be `rightNano`, the number of nanoseconds in the
        // denominator.
        // Let `B` be one billion.
        //
        // First we calculate `a * B / d` in such a way to minimize the chance
        // of overflow. Then we add `b / d` to this to arrive at the answer.
        const std::size_t a = leftSec;
        const long        b = leftNano;
        const long        d = rightNano;
        const long        B = CHAN_CAT(1, 000, 000, 000);

        const std::ldiv_t aDiv   = std::ldiv(a, d);
        const std::ldiv_t BDiv   = std::ldiv(B, d);
        const std::ldiv_t remDiv = std::ldiv(aDiv.rem * BDiv.rem, d);

        // clang-format off
        // By writing `a * B + b` in terms of the `quot` and `rem` of the
        // divisions above, you can convince yourself that then after dividing
        // by `d` you get:
        return aDiv.quot * BDiv.quot * d
             + aDiv.quot * BDiv.rem
             + BDiv.quot * aDiv.rem
             + remDiv.quot
             + b / d;
        // clang-format on
    }
    else {
        // Both the numerator (left) and the denominator (right) have seconds,
        // so we can just divide those (the nanoseconds would contribute only
        // to the remainder).
        return leftSec / rightSec;
    }
}

}  // namespace chan