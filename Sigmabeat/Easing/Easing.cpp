#include "Easing.h"
#include <math.h>

namespace easing {
	double ease(int time, int startTime, int endTime, double startValue, double endValue, easeParam param) {
		if (endTime <= startTime) return endValue;
		if (time < startTime) return startValue;
		if (time > endTime) return endValue;
		int   dur = time - startTime;
		double t = dur / (endTime - startTime);
		double c = endValue - startValue;
		switch (param) {
		case linear:  return c * t + startValue;
		case quadIn:  return c * t * t + startValue;
		case quadOut: return -c * t * (t - 2.f) + startValue;
		case quadInOut:
			t /= 0.5f;
			if (t < 1.0) return c / 2 * t * t + startValue;
			return -c / 2 * ((--t) * (t - 2) - 1) + startValue;
		case cubicIn:  return c * t * t * t + startValue;
		case cubicOut: return c * ((--t) * t * t + 1) + startValue;
		case cubicInOut:
			t /= 0.5f;
			if (t < 1) return c / 2 * t * t * t + startValue;
			t -= 2.f;
			return c / 2 * (t * t * t + 2) + startValue;
		case quartIn:  return c * t * t * t * t + startValue;
		case quartOut:
			return -c * ((--t) * t * t * t - 1) + startValue;
		case quartInOut:
			t /= 0.5f;
			if (t < 1) return c / 2 * t * t * t * t + startValue;
			t -= 2;
			return -c / 2 * (t * t * t * t - 2) + startValue;
		case quintIn:  return c * t * t * t * t * t + startValue;
		case quintOut: return c * ((--t) * t * t * t * t + 1) + startValue;
		case quintInOut:
			t /= 0.5f;
			if (t < 1) return c / 2 * t * t * t * t * t + startValue;
			t -= 2;
			return c / 2 * (t * t * t * t * t + 2) + startValue;
		case sineIn:    return -c * cos(t * (DX_PI / 2)) + c + startValue;
		case sineOut:   return c * sin(t * (DX_PI / 2)) + startValue;
		case sineInOut: return -c / 2 * (cos(DX_PI * t) - 1) + startValue;
		case expoIn:    return c * pow(2, 10 * (t - 1)) + startValue; // c * exp(6.931471805599453 * (t - 1)) + startValue;
		case expoOut:   return c * (-pow(2, -10 * t) + 1) + startValue;
		case expoInOut:
			t /= 0.5f;
			if (t < 1)  return c / 2 * pow(2, 10 * (t - 1)) + startValue;
			return c / 2 * (-pow(2, -10 * (--t)) + 2) + startValue;
		case circIn:    return -c * (sqrt(1 - t * t) - 1) + startValue;
		case circOut:   return c * sqrt(1 - (--t) * t) + startValue;
		case circInOut:
			t /= 0.5f;
			if (t < 1) return -c / 2 * (sqrt(1 - t * t) - 1) + startValue;
			t -= 2;
			return c / 2 * (sqrt(1 - t * t) + 1) + startValue;
		}
		return endValue;
	}
	double ease(Timer time, int startTime, int endTime, double startValue, double endValue, easeParam param) {
		return ease(time.Elapse(), startTime, endTime, startValue, endValue, param);
	}
}