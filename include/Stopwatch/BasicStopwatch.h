//	BasicStopwatch.h

#if !defined(BASIC_STOPWATCH_H)

#define BASIC_STOPWATCH_H

namespace ADL
{
	template<typename Counter>
	class BasicStopwatch
	{
	public:
		typedef typename Counter::Time Time;

	private:
		Time begin;
		Time total;

	public:
		BasicStopwatch() : begin(Time()), total(Time()) {
		}
		void Start() {
			begin = Counter::getTime();
		}
		void Pause() {
			if(begin == Time()) {	//	スタートした場合のみ計測
				return;
			}

			Time t = Counter::getTime();

			total += t - begin;
			begin = t;
		}
		void Stop() {
			Pause();
			begin = Time();
		}
		void Clear() {
			begin = total = Time();
		}
		Time Hour() const {
			return Minute() / 60;
		}
		Time Minute() const {
			return static_cast<Time>(Second() / 60);
		}
		double Second() const {
			return static_cast<double>(TotalTime()) / Counter::getFrequency();
		}
		Time TotalTime() const {
			return total;
		}
	};	//	BasicStopwatch
}	//	ADL

#endif	//	BASIC_STOPWATCH_H

