#pragma once
#include "../Time/Timer.h"

namespace easing {
	enum easeParam {
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
		circInOut,
		// Progress
		backIn,
		backOut,
		backInOut,

		elasticIn,
		elasticOut,
		elasticInOut,

		bounceIn,
		bounceOut,
		bounceInOut
    };

	double ease(int time, int startTime, int endTime, double startValue, double endValue, easeParam param);
	double ease(Timer time, int startTime, int endTime, double startValue, double endValue, easeParam param);
}