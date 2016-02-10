//	SingleThreadStopwatch.h

#if !defined(SINGLE_THREAD_STOPWATCH_H)

#define SINGLE_THREAD_STOPWATCH_H

#include "Stopwatch/BasicStopwatch.h"
#include "Stopwatch/QueryPerformanceCounterAdapter.h"

namespace ADL
{
	typedef BasicStopwatch<QueryPerformanceCounterAdapter> SingleThreadStopwatch;
}

#endif	//	SINGLE_THREAD_STOPWATCH_H

