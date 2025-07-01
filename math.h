#ifndef MATH_H_
#define MATH_H_
#include <stdbool.h>
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
  x *= 0.159154943092f;
  x -= 0.25f + _floor(x + 0.25f);
  x *= 16.f * (_abs(x) - 0.5f);
  return x;
}
static inline float _sin(float x) {
  return _cos(x + 1.57079632679f);
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
  float x_sq = x*x;
  return x * _ma(x_sq, _ma(x_sq, _ma(x_sq, _ma(x_sq, _ma(x_sq, a11, a9), a7), a5), a3), a1);
}
static inline float _atan2(float y, float x) {
  bool swap = _abs(x) < _abs(y);
  float atan_input = (swap ? x : y) / (swap ? y : x);
  float res = _atan(atan_input);
  res = swap ? (atan_input >= 0.f ? 6.28318530718f : -6.28318530718f) - res : res;
  if (x < 0.f) {
    res += (y >= 0.f ? 3.14159265359f : -3.14159265359f);
  }
  return res;
}
#endif
