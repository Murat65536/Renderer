#ifndef MATH_H_
#define MATH_H_
#define PI 3.1415926535f
#define TWO_PI 6.28318530718f
#define HALF_PI 1.57079632679f
#define THREE_HALF_PI 4.71238898038f
#define ONE_OVER_TWO_PI 0.159154943092f
#define FOUR_OVER_PI 1.27323954474f

static const unsigned char _pow10[3] = {1, 10, 100};
static inline float _min(float x, float y) {
  if (x < y) {
    return x;
  }
  else {
    return y;
  }
}
static inline float _max(float x, float y) {
  if (x > y) {
    return x;
  }
  else {
    return y;
  }
}
static inline float _clamp(float x, float min_x, float max_x) {
  if (x > max_x) {
    return max_x;
  }
  if (x < min_x) {
    return min_x;
  }
  return x;
}
static inline int _floor(float x) {
  int i = (int)x;
  return i - (i > x);
}
static inline int _ceil(float x) {
  int i = (int)x;
  return i + (i < x);
}
static inline float _abs(float x) {
  int casted = *(int*)&x;
  casted &= 0x7FFFFFFF;
  return *(float*)&casted;
}
static inline float _cos(float x) {
  x *= ONE_OVER_TWO_PI;
  x -= 0.25f + _floor(x + 0.25f);
  x *= 16.f * (_abs(x) - 0.5f);
  return x;
}
static inline float _sin(float x) {
  return _cos(x + HALF_PI);
}
static inline float _tans(float x) {
    return x * -3.6112171 / (-4.6133253 + x * x);
}
static inline float _tan(float x) {
    x = x - (int)(x / TWO_PI) * TWO_PI;
    int octant = (int)(x * FOUR_OVER_PI);
    switch (octant) {
        case 0:
            return _tans(x * FOUR_OVER_PI);
        case 1:
            return 1.f / _tans((HALF_PI - x) * FOUR_OVER_PI);
        case 2:
            return -1.f / _tans((x - HALF_PI) * FOUR_OVER_PI);
        case 3:
            return -_tans((PI - x) * FOUR_OVER_PI);
        case 4:
            return _tans((x - PI) * FOUR_OVER_PI);
        case 5:
            return 1.f / _tans((THREE_HALF_PI - x) * FOUR_OVER_PI);
        case 6:
            return -1.f / _tans((x - THREE_HALF_PI) * FOUR_OVER_PI);
        case 7:
            return -_tans((TWO_PI - x) * FOUR_OVER_PI);
        default:
            return 3.402823466e+38f;
    }
}
static inline float _sqrt(float x) {
  unsigned int i = *(unsigned int *) &x;
  i += 127 << 23;
  i >>=1;
  return *(float *)&i;
}
static inline float _ma(float x, float y, float z) {
  if (x > y) {
    if (x > z) {
      return x;
    }
    else {
      return z;
    }
  }
  else {
    if (y > z) {
      return y;
    }
    else {
      return z;
    }
  }
}
static inline float _atan(float x) {
  float a1 = 0.99997726f;
  float a3 = -0.33262347f;
  float a5 = 0.19354346f;
  float a7 = -0.11643287f;
  float a9 = 0.05265332f;
  float a11 = -0.01172120f;
  float x_sq = x * x;
  return x * _ma(x_sq, _ma(x_sq, _ma(x_sq, _ma(x_sq, _ma(x_sq, a11, a9), a7), a5), a3), a1);
}
static inline float _atan2(float y, float x) {
  bool swap = _abs(x) < _abs(y);
  float atan_input = (swap ? x : y) / (swap ? y : x);
  float res = _atan(atan_input);
  res = swap ? (atan_input >= 0.f ? TWO_PI : -TWO_PI) - res : res;
  if (x < 0.f) {
    res += (y >= 0.f ? PI : -PI);
  }
  return res;
}
#endif
