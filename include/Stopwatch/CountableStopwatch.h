//	CountableStopwatch.h

#ifndef COUNTABLE_STOPWATCH_H

#define COUNTABLE_STOPWATCH_H

namespace ADL
{
	template<typename T>
	class CountableStopwatch
	{
	public:
		typedef typename T Stopwatch;
		typedef typename Stopwatch::Time Time;

	private:
		Stopwatch stopwatch;
		int nCount;

	public:
		CountableStopwatch() : nCount(0) {
		}
		void Start() {
			stopwatch.Start();
			++nCount;
		}
		void Pause() {
			stopwatch.Pause();
		}
		void Stop() {
			stopwatch.Stop();
		}
		void Clear() {
			stopwatch.Clear();
			nCount = 0;
		}
		Time Hour() const {
			return stopwatch.Hour();
		}
		Time Minute() const {
			return stopwatch.Minute();
		}
		double Second() const {
			return stopwatch.Second();
		}
		int Count() const {
			return nCount;
		}
		Time TotalTime() const {
			return stopwatch.TotalTime();
		}
	};	//	CountableStopwatch
}	//	ADL

#endif	//	COUNTABLE_STOPWATCH_H

