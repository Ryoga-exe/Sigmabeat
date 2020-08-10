#include "Easing.h"
#include <math.h>

namespace easing {
    double ease(int time, int startTime, int endTime, double startValue, double endValue, easeParam param) {
        if (endTime <= startTime) return endValue;
        if (time < startTime) return startValue;
        if (time > endTime) return endValue;
        const double dur = (double)time - startTime;
        double t = dur / ((double)endTime - startTime);
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
            if (t < 1)  return -c / 2 * (sqrt(1 - t * t) - 1) + startValue;
            t -= 2;
            return c / 2 * (sqrt(1 - t * t) + 1) + startValue;
        case backIn:    return c * (2.70158 * t * t * t - 1.70158 * t * t) + startValue;
        case backOut:   return c * (1 + 2.70158 * pow(t - 1, 3) + 1.70158 * pow(t - 1, 2)) + startValue;
        case backInOut:
            return c * (t < 0.5
                ? (pow(2 * t, 2) * (((1.70158 * 1.525) + 1) * 2 * t - (1.70158 * 1.525))) / 2
                : (pow(2 * t - 2, 2) * (((1.70158 * 1.525) + 1) * (t * 2 - 2) + (1.70158 * 1.525)) + 2) / 2)
                + startValue;
        case elasticIn:  return c * (t == 0 ? 0 : t == 1 ? 1 : -pow(2, 10 * t - 10) * sin((t * 10 - 10.75) * ((2 * DX_PI) / 3))) + startValue;
        case elasticOut: return c * (t == 0 ? 0 : t == 1 ? 1 : pow(2, -10 * t) * sin((t * 10 - 0.75) * ((2 * DX_PI) / 3)) + 1) +startValue;
        case elasticInOut:
            return c * (t == 0 ? 0 : t == 1 ? 1 : t < 0.5
                ? -(pow(2, 20 * t - 10) * sin((20 * t - 11.125) * ((2 * DX_PI) / 4.5))) / 2
                : (pow(2, -20 * t + 10) * sin((20 * t - 11.125) * ((2 * DX_PI) / 4.5))) / 2 + 1) + startValue;
        case bounceIn:
            t = 1 - t;
            c *= -1;
        case bounceOut:
            return (param == bounceIn ? -c : 0)
                + c * (t < 1 / 2.75
                ? 7.5625 * t * t : t < 2 / 2.75
                ? 7.5625 * (t -= 1.5 / 2.75) * t + 0.75 : t < 2.5 / 2.75
                ? 7.5625 * (t -= 2.25 / 2.75) * t + 0.9375
                : 7.5625 * (t -= 2.625 / 2.75) * t + 0.984375)
                + startValue;
        case bounceInOut:
            return t < 0.5
                ? (c - ease((int)((1 - 2 * t) * 1000), 0, 1000, 0, c, bounceOut)) / 2 + startValue
                : (c + ease((int)((2 * t - 1) * 1000), 0, 1000, 0, c, bounceOut)) / 2 + startValue;
        }
        return endValue;
    }
    double ease(Timer time, int startTime, int endTime, double startValue, double endValue, easeParam param) {
        return ease(time.Elapse(), startTime, endTime, startValue, endValue, param);
    }
}