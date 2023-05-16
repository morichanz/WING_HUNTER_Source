/**
* Easing.h
*/
#ifndef EASING_H_INCLUDE
#define EASING_H_INCLUDE

class Easing
{
public:
	inline static float EaseOutExpo( float val)
	{
		const float num = -10.0f;

		return (val == 1.0f ? 1.0f : 1.0f - pow(2, num * val));
	}
	inline static float EaseOutBack(float val)
	{
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.0f;

		return (1.0f + c3 * pow(val - 1.0f, 3) + c1 * pow(val - 1.0f, 2));
	}
	inline static float EaseOutQuart(float val)
	{
		return(1.0f - pow(1.0f - val, 4));
	}
	inline static float EaseInOutQuart(float val)
	{
		float c1 = 8.0f * val * val * val * val;
		return(val < 0.5f ? c1 : 1.0f - pow(-2.0f * val + 2.0f, 4.0f) / 2);
	}
	inline static float  EaseInOutCubic(float val)
	{
		float c1 = 4.0f * val * val * val;
		return(val < 0.5f ? c1 : 1.0f - pow(-2.0f * val + 2.0f, 3) / 2);
	}
	inline static float  EaseInOutBack(float val)
	{
		const float c1 = 1.70158f;
		const float c2 = c1 * 1.525f;
		const float c3 = 0.5f;
		float c4 = (c2 + 1.0f) * 2 * val - c2;
		float c5 = (c2 + 1.0f) * (val * 2.0f - 2.0f) + c2;

		return val < c3
		  ? (pow(2.0f * val, 2) * (c4)) / 2.0f
		  : (pow(2.0f * val - 2.0f, 2) * (c5) + 2.0f) / 2.0f;
	}

	inline static float EaseOutCirc(float val)
	{
		return sqrt(1.0f - pow(val - 1.0f, 2));
	}
};
#endif // !EASING_H_INCLUDE