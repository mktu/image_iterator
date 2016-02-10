--------------------------------------------------------------------------------------------------------------------------------
SingleThreadStopwatch
	
	�V���O���X���b�h�p�X�g�b�v�E�H�b�`
	
	typedef BasicStopwatch<QueryPerformanceCounterAdapter> SingleThreadStopwatch
	
	�����o�F
		void Start()
			���Ԍv�����J�n����
		
		void Stop()
			���Ԍv�����I�����A�ݐώ��Ԃ��X�V����
		
		void Clear()
			�X�g�b�v�E�H�b�`������������
		
		long Hour() const
			�v�����Ԃ̗݌v�����ԒP�ʂŕԂ��i�[���͐؂�̂āj

		long Minute() const
			�v�����Ԃ̗݌v�𕪒P�ʂŕԂ��i�[���͐؂�̂āj

		double Second() const
			�v�����Ԃ̗݌v��b�P�ʂŕԂ�
	
	����F
		SingleThreadStopwatch�̓e���v���[�g�N���XBasicStopwatch����ꉻ�������̂̕ʖ��ł���B
		
		Start()����Stop()�܂ł̎��Ԃ��v�����A���̎��Ԃ̗ݐς��L�^����B
		
		���Ԃ̌v���ɂ�QueryPerformanceCounter()���g�p���Ă��邽�߁A�v�����Ԃ̐��x�͍������A
		���荞�݂ɂ���Đ����鏈���̒��f���Ԃ��L�^�����B
		
		���荞�݂ɂ�钆�f�̉e�����傫���ꍇ�́AMultiThreadStopwatch���g�p����B
		�i�ڍׂ�MultiThreadStopwatch���Q�Ɓj
		
	�T���v���R�[�h�F
		#include <ADL/Stopwatch/SingleThreadStopwatch.h>
		
		ADL::SingleThreadStopwatch stopwatch;
		
		�E�E�E
		
		{
			for(int i = 0; i < 100; ++i) {
				
				�E�E�E
				
				stopwatch.Start();
				
				�E�E�E
				
				stopwatch.Stop();
				
				�E�E�E
				
			}
			
			long nHr = stopwatch.Hour();
			long nMin = stopwatch.Minute() - (nHr * 60);
			double dSec = (stopwatch.Second() - (nMin * 60));
			
			std::cout << nHr << ":" << nMin << ":" << dSec << std::endl;
		}
		
		�E�E�E

--------------------------------------------------------------------------------------------------------------------------------
MultiThreadStopwatch
	
	�V���O���X���b�h�p�X�g�b�v�E�H�b�`
	
	typedef BasicStopwatch<GetThreadTimes> MultiThreadStopwatch
	
	�����o�F
		void Start()
			���Ԍv�����J�n����
		
		void Stop()
			���Ԍv�����I�����A�ݐώ��Ԃ��X�V����
		
		void Clear()
			�X�g�b�v�E�H�b�`������������
		
		long Hour() const
			�v�����Ԃ̗݌v�����ԒP�ʂŕԂ��i�[���͐؂�̂āj
		
		long Minute() const
			�v�����Ԃ̗݌v�𕪒P�ʂŕԂ��i�[���͐؂�̂āj
		
		double Second() const
			�v�����Ԃ̗݌v��b�P�ʂŕԂ�
	
	����F
		MultiThreadStopwatch�̓e���v���[�g�N���XBasicStopwatch����ꉻ�������̂̕ʖ��ł���B
		
		Start()����Stop()�܂ł̎��Ԃ��v�����A���̎��Ԃ̗ݐς��L�^����B
		
		���Ԃ̌v���ɂ�GetThreadTimes()���g�p���Ă��邽�߁A���荞�݂ɂ���Đ����鏈���̒��f���Ԃ�
		�L�^���Ȃ����A�v�����Ԃ̐��x�͒Ⴂ�B
		
		���荞�݂ɂ�钆�f�����v�����Ԃ̐��x���d�v�ȏꍇ�́ASingleThreadStopwatch���g�p����B
		�i�ڍׂ�SingleThreadStopwatch���Q�Ɓj
		
	�T���v���R�[�h�F
		#include <ADL/Stopwatch/MultiThreadStopwatch.h>
		
		ADL::MultiThreadStopwatch stopwatch;
		
		�E�E�E
		
		{
			for(int i = 0; i < 100; ++i) {
				
				�E�E�E
				
				stopwatch.Start();
				
				�E�E�E
				
				stopwatch.Stop();
				
				�E�E�E
				
			}
			
			long nHr = stopwatch.Hour();
			long nMin = stopwatch.Minute() - (nHr * 60);
			double dSec = (stopwatch.Second() - (nMin * 60));
			
			std::cout << nHr << ":" << nMin << ":" << dSec << std::endl;
		}
		
		�E�E�E


	���F
		GetThreadTimes()�Ɋւ��Ĉȉ��̃R�����g���������̂ŋL���B
		�ihttp://blog.kalmbachnet.de/?postid=28 ���甲���j

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
		NtQueryInformationThread then gets the Thread�Ls Thread-Information-Block 
		(TCB or TIB) and retrives the Kernel- and User-Times from the TCB and then 
		it will multiply with the �gnt!KeMaximumIncrement�h and return the result.

		You can also dig into GetSystemTimeAdjustment and you will see that for 
		�glpTimeIncrement�h the same value is retrived as which is used in the 
		multiply operation in �gNtQueryInformationThread�h...

		Conclusion
		The resolution of the returned values is never better than the �gTimeIncrement�h 
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
		call �gSleep(1)�h, which will apruptly end our qunatum and will reschedule or thread 
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
	
	�V���O���X���b�h�p�Ăяo���J�E���^�t�X�g�b�v�E�H�b�`
	
	typedef CountableStopwatch<SingleThreadStopwatch> CountableSingleThreadStopwatch
	
	�����o�F
		void Start()
			���Ԍv�����J�n���A�Ăяo���J�E���^���C���N�������g����
		
		void Stop()
			���Ԍv�����I�����A�ݐώ��Ԃ��X�V����
		
		void Clear()
			�X�g�b�v�E�H�b�`�ƌĂяo���J�E���^������������
		
		long Hour() const
			�v�����Ԃ̗݌v�����ԒP�ʂŕԂ��i�[���͐؂�̂āj

		long Minute() const
			�v�����Ԃ̗݌v�𕪒P�ʂŕԂ��i�[���͐؂�̂āj

		double Second() const
			�v�����Ԃ̗݌v��b�P�ʂŕԂ�
			
		int Count() const
			�Ăяo���J�E���^�̒l��Ԃ�
	
	����F
		CountableSingleThreadStopwatch�̓e���v���[�g�N���XCountableStopwatch�Ƀe���v���[�g����
		�Ƃ���SingleThreadStopwatch���w�肵�����̂̕ʖ��ł���B
		
		Start()����Stop()�܂ł̎��Ԃ��v�����A���̎��Ԃ̗ݐς��L�^����B�܂��AStart()�Ăяo���̍�
		�Ăяo���J�E���^��1�C���N�������g����B
		
		���Ԃ̌v���ɂ�SingleThreadStopwatch���g�p���Ă��邽�߁A�v�����Ԃ̐��x�͍������A
		���荞�݂ɂ���Đ����鏈���̒��f���Ԃ��L�^�����B
		�i�ڍׂ�SingleThreadStopwatch���Q�Ɓj
		
		���荞�݂ɂ�钆�f�̉e�����傫���ꍇ�́ACountableMultiThreadStopwatch���g�p����B
		�i�ڍׂ�CountableMultiThreadStopwatch���Q�Ɓj
		
	�T���v���R�[�h�F
		#include <ADL/Stopwatch/SingleThreadStopwatch.h>
		
		ADL::CountableSingleThreadStopwatch stopwatch;
		
		�E�E�E
		
		{
			for(int i = 0; i < 100; ++i) {
				
				�E�E�E
				
				stopwatch.Start();
				
				�E�E�E
				
				stopwatch.Stop();
				
				�E�E�E
				
			}
			
			long nHr = stopwatch.Hour();
			long nMin = stopwatch.Minute() - (nHr * 60);
			double dSec = (stopwatch.Second() - (nMin * 60));
			int nCount = stopwatch.Count();
			
			std::cout << nHr << ":" << nMin << ":" << dSec << " (" << nCount << ")" << std::endl;
		}
		
		�E�E�E

--------------------------------------------------------------------------------------------------------------------------------
CountableMultiThreadStopwatch
	
	�V���O���X���b�h�p�X�g�b�v�E�H�b�`
	
	typedef CountableStopwatch<MultiThreadStopwatch> CountableMultiThreadStopwatch
	
	�����o�F
		void Start()
			���Ԍv�����J�n����
		
		void Stop()
			���Ԍv�����I�����A�ݐώ��Ԃ��X�V����
		
		void Clear()
			�X�g�b�v�E�H�b�`������������
		
		long Hour() const
			�v�����Ԃ̗݌v�����ԒP�ʂŕԂ��i�[���͐؂�̂āj
		
		long Minute() const
			�v�����Ԃ̗݌v�𕪒P�ʂŕԂ��i�[���͐؂�̂āj
		
		double Second() const
			�v�����Ԃ̗݌v��b�P�ʂŕԂ�
	
	����F
		CountableMultiThreadStopwatch�̓e���v���[�g�N���XCountableStopwatch�Ƀe���v���[�g����
		�Ƃ���MultiThreadStopwatch���w�肵�����̂̕ʖ��ł���B
		
		Start()����Stop()�܂ł̎��Ԃ��v�����A���̎��Ԃ̗ݐς��L�^����B�܂��AStart()�Ăяo���̍�
		�Ăяo���J�E���^��1�C���N�������g����B
				
		Start()����Stop()�܂ł̎��Ԃ��v�����A���̎��Ԃ̗ݐς��L�^����B
		
		���Ԃ̌v���ɂ�MultiThreadStopwatch���g�p���Ă��邽�߁A���荞�݂ɂ���Đ����鏈���̒��f���Ԃ�
		�L�^���Ȃ����A�v�����Ԃ̐��x�͒Ⴂ�B
		
		���荞�݂ɂ�钆�f�����v�����Ԃ̐��x���d�v�ȏꍇ�́ACountableSingleThreadStopwatch���g�p����B
		�i�ڍׂ�CountableSingleThreadStopwatch���Q�Ɓj
		
	�T���v���R�[�h�F
		#include <ADL/Stopwatch/MultiThreadStopwatch.h>
		
		ADL::CountableMultiThreadStopwatch stopwatch;
		
		�E�E�E
		
		{
			for(int i = 0; i < 100; ++i) {
				
				�E�E�E
				
				stopwatch.Start();
				
				�E�E�E
				
				stopwatch.Stop();
				
				�E�E�E
				
			}
			
			long nHr = stopwatch.Hour();
			long nMin = stopwatch.Minute() - (nHr * 60);
			int nCount = stopwatch.Count();
			
			std::cout << nHr << ":" << nMin << ":" << dSec << " (" << nCount << ")" << std::endl;
		}
		
		�E�E�E

--------------------------------------------------------------------------------------------------------------------------------
