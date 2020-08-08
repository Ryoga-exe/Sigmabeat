#pragma once
#include "../Time/Timer.h"

namespace easing {
    enum easeParam{
		linear,
		quadIn,
		quadOut,
		quadInOut,
		cubicIn,
		cubicOut,
		cubicInOut,
		quartIn,
		quartOut,
		quartInOut,
		quintIn,
		quintOut,
		quintInOut,
		sineIn,
		sineOut,
		sineInOut,
		expoIn,
		expoOut,
		expoInOut,
		circIn,
		circOut,
		circInOut
    };

	double ease(int time, int startTime, int endTime, float startValue, float endValue, easeParam param);
	double ease(Timer time, int startTime, int endTime, float startValue, float endValue, easeParam param);
}