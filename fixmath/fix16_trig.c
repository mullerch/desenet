#include "fix16.h"

#if defined(FIXMATH_SIN_LUT)
#include "fix16_trig_sin_lut.h"
#elif !defined(FIXMATH_NO_CACHE)
static fix16_t _fix16_sin_cache_index[4096]  = { 0 };
static fix16_t _fix16_sin_cache_value[4096]  = { 0 };
#endif

#ifndef FIXMATH_NO_CACHE
static fix16_t _fix16_atan_cache_index[2][4096] = { { 0 }, { 0 } };
static fix16_t _fix16_atan_cache_value[4096] = { 0 };
#endif



fix16_t fix16_sin(fix16_t inAngle) {
	fix16_t tempAngle = inAngle % (fix16_pi << 1);

	#ifdef FIXMATH_SIN_LUT
	if(tempAngle < 0)
		tempAngle += (fix16_pi << 1);

	fix16_t tempOut;
	if(tempAngle >= fix16_pi) {
		tempAngle -= fix16_pi;
		if(tempAngle >= (fix16_pi >> 1))
			tempAngle = fix16_pi - tempAngle;
		tempOut = -(tempAngle >= _fix16_sin_lut_count ? fix16_one : _fix16_sin_lut[tempAngle]);
	} else {
		if(tempAngle >= (fix16_pi >> 1))
			tempAngle = fix16_pi - tempAngle;
		tempOut = (tempAngle >= _fix16_sin_lut_count ? fix16_one : _fix16_sin_lut[tempAngle]);
	}
	#else
	if(tempAngle > fix16_pi)
		tempAngle -= (fix16_pi << 1);
	else if(tempAngle < -fix16_pi)
		tempAngle += (fix16_pi << 1);

	#ifndef FIXMATH_NO_CACHE
	fix16_t tempIndex = ((inAngle >> 5) & 0x00000FFF);
	if(_fix16_sin_cache_index[tempIndex] == inAngle)
		return _fix16_sin_cache_value[tempIndex];
	#endif

	fix16_t tempAngleSq = fix16_mul(tempAngle, tempAngle);

	#ifndef FIXMATH_FAST_SIN // Most accurate version, accurate to ~2.1%
	fix16_t tempOut = tempAngle;
	tempAngle = fix16_mul(tempAngle, tempAngleSq);
	tempOut -= (tempAngle / 6);
	tempAngle = fix16_mul(tempAngle, tempAngleSq);
	tempOut += (tempAngle / 120);
	tempAngle = fix16_mul(tempAngle, tempAngleSq);
	tempOut -= (tempAngle / 5040);
	tempAngle = fix16_mul(tempAngle, tempAngleSq);
	tempOut += (tempAngle / 362880);
	tempAngle = fix16_mul(tempAngle, tempAngleSq);
	tempOut -= (tempAngle / 39916800);
	#else // Fast implementation, runs at 159% the speed of above 'accurate' version with an slightly lower accuracy of ~2.3%
	fix16_t tempOut;
	tempOut = fix16_mul(-13, tempAngleSq) + 546;
	tempOut = fix16_mul(tempOut, tempAngleSq) - 10923;
	tempOut = fix16_mul(tempOut, tempAngleSq) + 65536;
	tempOut = fix16_mul(tempOut, tempAngle);
	#endif

	#ifndef FIXMATH_NO_CACHE
	_fix16_sin_cache_index[tempIndex] = inAngle;
	_fix16_sin_cache_value[tempIndex] = tempOut;
	#endif
	#endif

	return tempOut;
}

fix16_t fix16_cos(fix16_t inAngle) {
	return fix16_sin(inAngle + (fix16_pi >> 1));
}
