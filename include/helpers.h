#ifndef TRIANGLES_SRC_HELPERS_H
#define TRIANGLES_SRC_HELPERS_H

#include <cmath>
#include <cstdint>

using real_t = double;

namespace helpers {

constexpr real_t PRECISION = 0.00000001;

inline bool _equalreal_t(real_t a, real_t b) {
    return ::abs(a - b) < PRECISION;
}
inline bool _lessreal_t(real_t a, real_t b) {
    return !_equalreal_t(a, b) && a < b;
}   
inline bool _greaterreal_t(real_t a, real_t b) {
    return !_equalreal_t(a, b) && a > b;
}   
inline bool _lessEqualreal_t(real_t a, real_t b) {
    return _equalreal_t(a, b) || _lessreal_t(a, b);
}
inline bool _greaterEqualreal_t(real_t a, real_t b) {
    return _equalreal_t(a, b) || _greaterreal_t(a, b);
}
inline int32_t _signum(real_t x) {
    if (_lessreal_t(x, 0)) return -1;
    if (_lessreal_t(0, x)) return 1;
    return 0;
}
}

#endif //TRIANGLES_SRC_HELPERS_H