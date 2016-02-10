//	GetThreadTimesAdapter.h

#if !defined(GET_THREAD_TIMES_ADAPTER_H)

#define GET_THREAD_TIMES_ADAPTER_H

#include <windows.h>

namespace ADL
{
	struct GetThreadTimesAdapter
	{
		typedef __int64 Time;

		static Time getTime() {
			FILETIME dummy, kernelTime, userTime;
			GetThreadTimes(GetCurrentThread(), &dummy, &dummy, &kernelTime, &userTime);
			return toTime(kernelTime) + toTime(userTime);
		}

		static double getFrequency() {
//	GetThreadTimes()‚Í100ƒiƒm•b’PˆÊ
			return 1.0e+7;
		}

	private:
		static Time toTime(const FILETIME& t) {
			return (static_cast<Time>(t.dwHighDateTime) << (sizeof(t.dwHighDateTime) * 8)) | t.dwLowDateTime;
		}
	};
}

#endif	//	GET_THREAD_TIMES_ADAPTER_H

