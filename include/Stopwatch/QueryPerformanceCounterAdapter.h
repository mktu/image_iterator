//	QueryPerformanceCounterAdapter.h

#if !defined(QUERY_PERFORMANCE_COUNTER_ADPTER_H)

#define QUERY_PERFORMANCE_COUNTER_ADPTER_H

#include <windows.h>

namespace ADL
{
	struct QueryPerformanceCounterAdapter
	{
		typedef __int64 Time;

		static Time getTime() {
			LARGE_INTEGER val = { 0 };
			QueryPerformanceCounter(&val);
			return val.QuadPart;
		}

		static Time getFrequency() {
			static const Time val = _QueryPerformanceFrequency();
			return val;
		}

	private:
		static Time _QueryPerformanceFrequency() {
			LARGE_INTEGER val = { 0 };
			QueryPerformanceFrequency(&val);
			return val.QuadPart;
		}
	};	//	QueryPerformanceCounterAdapter
}

#endif	//	QUERY_PERFORMANCE_COUNTER_ADPTER_H

