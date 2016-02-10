#include <gtest/gtest.h>
#include <Windows.h>
#include <algorithm>

int main(int argc, TCHAR* argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}