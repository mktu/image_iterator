#pragma once

#include <iostream>
#include <string>

#ifdef _RELEASE
#include <gtest/gtest.h>
#include <SingleThreadStopwatch.h>
using namespace ADL;

struct timer_on
{
	timer_on()
		: name(::testing::UnitTest::GetInstance()->current_test_info()->name())
	{
		sw.Start();
	}

	~timer_on()
	{
		sw.Stop();

		std::cout << name
			<<">>>Execution time :" << static_cast<long>(sw.Second() * 1000) 
			<< "(msec) <<<" << std::endl;
	}
private:
	std::string name;
	SingleThreadStopwatch sw;
};
#else
struct timer_on{};
#endif



