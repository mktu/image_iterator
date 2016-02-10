--------------------------------------------------------------------------------------------------------------------------------
SingleThreadStopwatch
	
	シングルスレッド用ストップウォッチ
	
	typedef BasicStopwatch<QueryPerformanceCounterAdapter> SingleThreadStopwatch
	
	メンバ：
		void Start()
			時間計測を開始する
		
		void Stop()
			時間計測を終了し、累積時間を更新する
		
		void Clear()
			ストップウォッチを初期化する
		
		long Hour() const
			計測時間の累計を時間単位で返す（端数は切り捨て）

		long Minute() const
			計測時間の累計を分単位で返す（端数は切り捨て）

		double Second() const
			計測時間の累計を秒単位で返す
	
	解説：
		SingleThreadStopwatchはテンプレートクラスBasicStopwatchを特殊化したものの別名である。
		
		Start()からStop()までの時間を計測し、その時間の累積を記録する。
		
		時間の計測にはQueryPerformanceCounter()を使用しているため、計測時間の精度は高いが、
		割り込みによって生じる処理の中断時間も記録される。
		
		割り込みによる中断の影響が大きい場合は、MultiThreadStopwatchを使用する。
		（詳細はMultiThreadStopwatchを参照）
		
	サンプルコード：
		#include <ADL/Stopwatch/SingleThreadStopwatch.h>
		
		ADL::SingleThreadStopwatch stopwatch;
		
		・・・
		
		{
			for(int i = 0; i < 100; ++i) {
				
				・・・
				
				stopwatch.Start();
				
				・・・
				
				stopwatch.Stop();
				
				・・・
				
			}
			
			long nHr = stopwatch.Hour();
			long nMin = stopwatch.Minute() - (nHr * 60);
			double dSec = (stopwatch.Second() - (nMin * 60));
			
			std::cout << nHr << ":" << nMin << ":" << dSec << std::endl;
		}
		
		・・・

--------------------------------------------------------------------------------------------------------------------------------
MultiThreadStopwatch
	
	シングルスレッド用ストップウォッチ
	
	typedef BasicStopwatch<GetThreadTimes> MultiThreadStopwatch
	
	メンバ：
		void Start()
			時間計測を開始する
		
		void Stop()
			時間計測を終了し、累積時間を更新する
		
		void Clear()
			ストップウォッチを初期化する
		
		long Hour() const
			計測時間の累計を時間単位で返す（端数は切り捨て）
		
		long Minute() const
			計測時間の累計を分単位で返す（端数は切り捨て）
		
		double Second() const
			計測時間の累計を秒単位で返す
	
	解説：
		MultiThreadStopwatchはテンプレートクラスBasicStopwatchを特殊化したものの別名である。
		
		Start()からStop()までの時間を計測し、その時間の累積を記録する。
		
		時間の計測にはGetThreadTimes()を使用しているため、割り込みによって生じる処理の中断時間は
		記録しないが、計測時間の精度は低い。
		
		割り込みによる中断よりも計測時間の精度が重要な場合は、SingleThreadStopwatchを使用する。
		（詳細はSingleThreadStopwatchを参照）
		
	サンプルコード：
		#include <ADL/Stopwatch/MultiThreadStopwatch.h>
		
		ADL::MultiThreadStopwatch stopwatch;
		
		・・・
		
		{
			for(int i = 0; i < 100; ++i) {
				
				・・・
				
				stopwatch.Start();
				
				・・・
				
				stopwatch.Stop();
				
				・・・
				
			}
			
			long nHr = stopwatch.Hour();
			long nMin = stopwatch.Minute() - (nHr * 60);
			double dSec = (stopwatch.Second() - (nMin * 60));
			
			std::cout << nHr << ":" << nMin << ":" << dSec << std::endl;
		}
		
		・・・


	注：
		GetThreadTimes()に関して以下のコメントを見つけたので記す。
		（http://blog.kalmbachnet.de/?postid=28 から抜粋）

		Why GetThreadTimes is wrong
		Indrodution
		Some applications are using GetThreadTimes for doing time measurement. 
		Most people does not know: GetThreadTimes can produce incorrect values!

		Some background
		To verify this statement, we need to know, how GetThreadTimes is working. 

		kernel32.dll
		Internally GetThreadTimes calls NtQueryInformationThread with ThreadTimes. 
		This returns the THREAD_TIMES_INFORMATION structure.

		ntoskrnl.exe
		NtQueryInformationThread then gets the Thread´s Thread-Information-Block 
		(TCB or TIB) and retrives the Kernel- and User-Times from the TCB and then 
		it will multiply with the “nt!KeMaximumIncrement” and return the result.

		You can also dig into GetSystemTimeAdjustment and you will see that for 
		“lpTimeIncrement” the same value is retrived as which is used in the 
		multiply operation in “NtQueryInformationThread”...

		Conclusion
		The resolution of the returned values is never better than the “TimeIncrement” 
		of the OS. On single-processor systems this is normally 10 ms; on multi-processor-systems 
		(and hypterthreading) it is normally 15 ms.

		More investigation
		If you dig deeper into the kernel, you find out, that in the timer- (or scheduler-) 
		interrupt (normally every 10/15 ms), the increment of the thread-time values 
		in the current running thread (TCB) is taken place.

		This leads to the conculsion, that GetThreadTimes will only produce correct 
		values, if each thread would consume all of its quantum (or at least for the 
		whole time-slice)! So, a thread should never go into wait-states or should be 
		interrupted by other (higher) priority threads. This always leads to wrong 
		values in GetThreadTimes. And you can imagine: In most of the times, your thread 
		will either go into wait-states (best examples are UI programs) or will be 
		interrupted by other (higher priority) threads or interrupts.

		Example
		With this knowlegde, we can easily produce an example, in which GetThreadTimes 
		will return 0 (zero) times for this thread, even if the thread was running for 
		many seconds or minutes! The only think we must do, is to end our thread quantum, 
		before the time tick is executed.

		We can achieve this by doing a very short calculation (for example 2 ms) and the 
		call “Sleep(1)”, which will apruptly end our qunatum and will reschedule or thread 
		the next time the timer-tick is over. And this we reapeat many times (for example 
		10000; then we have used the CPU for at least 20 seconds; but GetThreadTimes will 
		return 0!). To be sure that the scheduler will not reschedule our thread immediately, 
		we need a second thread which must be ready-to-run all the time (and the times are now 
		counted to this thread).

		To cut a long story short: Here is the code:


		  #include <windows.h>
		  #include <stdio.h>

		  DWORD loopCounter = 0;
		  DWORD loopCounterMax = 1000;
		  DWORD internalCounter = 0xFFF00000;

		  DWORD __stdcall CalcThread(LPVOID)
		  {
			while(loopCounter <= loopCounterMax)
			{
			  DWORD cnt = internalCounter;
			  while(cnt != 0) cnt++;
			  Sleep(1);
			  loopCounter++;
			} return 0;
		  }

		  DWORD WINAPI IdleThread(LPVOID)
		  {
			while(loopCounter <= loopCounterMax)
			{
			  Sleep(0);  // just do something...
			} return 0;
		  }

		  int _tmain(int argc, _TCHAR* argv[])
		  {
			// be sure we only use 1 processor!
			SetProcessAffinityMask(GetCurrentProcess(), 1);

			LARGE_INTEGER liStart, liEnd, liFreq;

			// test, how much time the inc is using...
			QueryPerformanceCounter(&liStart);
			DWORD cnt = internalCounter;
			while(cnt != 0) cnt++;
			QueryPerformanceCounter(&liEnd);
			QueryPerformanceFrequency(&liFreq);
			double ms = ((double) (liEnd.QuadPart-liStart.QuadPart) * 1000) /
		  (double)liFreq.QuadPart;
			printf("Inc duration: %.3f msnn", ms);
			// test-end

			DWORD id;
			HANDLE hThread[ 2 ];
			QueryPerformanceCounter(&liStart);

			hThread[ 0 ] = CreateThread(NULL, 0, CalcThread, 0, 0, &id);
			hThread[ 1 ] = CreateThread(NULL, 0, IdleThread, 0, 0, &id);

			WaitForMultipleObjects(2, hThread, TRUE, INFINITE);

			QueryPerformanceCounter(&liEnd);
			QueryPerformanceFrequency(&liFreq);

			ms = ((double) (liEnd.QuadPart-liStart.QuadPart) * 1000) / (double)
		  liFreq.QuadPart;
			printf("Duration: %.3f msnn", ms);

			FILETIME ftCreate, ftExit, ftKernel, ftUser;
			for(DWORD i=0; i<2; i++)
			{
			  GetThreadTimes(hThread[i], &ftCreate, &ftExit, &ftKernel, &ftUser);

			  printf("Reported time for thread %dn", i+1);

			  SYSTEMTIME st;
			  FileTimeToSystemTime(&ftKernel, &st);
			  printf("Kernel: %2.2d:%2.2d.%3.3dn", st.wMinute, st.wSecond,
		  st.wMilliseconds);

			  FileTimeToSystemTime(&ftUser, &st);
			  printf("User: %2.2d:%2.2d.%3.3dnn", st.wMinute, st.wSecond,
		  st.wMilliseconds);
			}
		  return 0;
		  }h5. And here is the output 

		  timeAdjustment: 156250, timeIncrement: 156250, timeAdjustmentDisabled: 1
		  Inc duration: 2.404 ms
		  Duration: 15679.180 ms

		  Reported time for thread 1
			Kernel: 00:00.000
			User: 00:00.000

		  Reported time for thread 2
			Kernel: 00:13.093
			User: 00:02.437

		And you see: thread 1 has 0 (zero) in its thread times (even it was running for 2 seconds)!
--------------------------------------------------------------------------------------------------------------------------------
CountableSingleThreadStopwatch
	
	シングルスレッド用呼び出しカウンタ付ストップウォッチ
	
	typedef CountableStopwatch<SingleThreadStopwatch> CountableSingleThreadStopwatch
	
	メンバ：
		void Start()
			時間計測を開始し、呼び出しカウンタをインクリメントする
		
		void Stop()
			時間計測を終了し、累積時間を更新する
		
		void Clear()
			ストップウォッチと呼び出しカウンタを初期化する
		
		long Hour() const
			計測時間の累計を時間単位で返す（端数は切り捨て）

		long Minute() const
			計測時間の累計を分単位で返す（端数は切り捨て）

		double Second() const
			計測時間の累計を秒単位で返す
			
		int Count() const
			呼び出しカウンタの値を返す
	
	解説：
		CountableSingleThreadStopwatchはテンプレートクラスCountableStopwatchにテンプレート引数
		としてSingleThreadStopwatchを指定したものの別名である。
		
		Start()からStop()までの時間を計測し、その時間の累積を記録する。また、Start()呼び出しの際
		呼び出しカウンタを1つインクリメントする。
		
		時間の計測にはSingleThreadStopwatchを使用しているため、計測時間の精度は高いが、
		割り込みによって生じる処理の中断時間も記録される。
		（詳細はSingleThreadStopwatchを参照）
		
		割り込みによる中断の影響が大きい場合は、CountableMultiThreadStopwatchを使用する。
		（詳細はCountableMultiThreadStopwatchを参照）
		
	サンプルコード：
		#include <ADL/Stopwatch/SingleThreadStopwatch.h>
		
		ADL::CountableSingleThreadStopwatch stopwatch;
		
		・・・
		
		{
			for(int i = 0; i < 100; ++i) {
				
				・・・
				
				stopwatch.Start();
				
				・・・
				
				stopwatch.Stop();
				
				・・・
				
			}
			
			long nHr = stopwatch.Hour();
			long nMin = stopwatch.Minute() - (nHr * 60);
			double dSec = (stopwatch.Second() - (nMin * 60));
			int nCount = stopwatch.Count();
			
			std::cout << nHr << ":" << nMin << ":" << dSec << " (" << nCount << ")" << std::endl;
		}
		
		・・・

--------------------------------------------------------------------------------------------------------------------------------
CountableMultiThreadStopwatch
	
	シングルスレッド用ストップウォッチ
	
	typedef CountableStopwatch<MultiThreadStopwatch> CountableMultiThreadStopwatch
	
	メンバ：
		void Start()
			時間計測を開始する
		
		void Stop()
			時間計測を終了し、累積時間を更新する
		
		void Clear()
			ストップウォッチを初期化する
		
		long Hour() const
			計測時間の累計を時間単位で返す（端数は切り捨て）
		
		long Minute() const
			計測時間の累計を分単位で返す（端数は切り捨て）
		
		double Second() const
			計測時間の累計を秒単位で返す
	
	解説：
		CountableMultiThreadStopwatchはテンプレートクラスCountableStopwatchにテンプレート引数
		としてMultiThreadStopwatchを指定したものの別名である。
		
		Start()からStop()までの時間を計測し、その時間の累積を記録する。また、Start()呼び出しの際
		呼び出しカウンタを1つインクリメントする。
				
		Start()からStop()までの時間を計測し、その時間の累積を記録する。
		
		時間の計測にはMultiThreadStopwatchを使用しているため、割り込みによって生じる処理の中断時間は
		記録しないが、計測時間の精度は低い。
		
		割り込みによる中断よりも計測時間の精度が重要な場合は、CountableSingleThreadStopwatchを使用する。
		（詳細はCountableSingleThreadStopwatchを参照）
		
	サンプルコード：
		#include <ADL/Stopwatch/MultiThreadStopwatch.h>
		
		ADL::CountableMultiThreadStopwatch stopwatch;
		
		・・・
		
		{
			for(int i = 0; i < 100; ++i) {
				
				・・・
				
				stopwatch.Start();
				
				・・・
				
				stopwatch.Stop();
				
				・・・
				
			}
			
			long nHr = stopwatch.Hour();
			long nMin = stopwatch.Minute() - (nHr * 60);
			int nCount = stopwatch.Count();
			
			std::cout << nHr << ":" << nMin << ":" << dSec << " (" << nCount << ")" << std::endl;
		}
		
		・・・

--------------------------------------------------------------------------------------------------------------------------------
