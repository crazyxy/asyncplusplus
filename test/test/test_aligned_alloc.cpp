#include "gtest/gtest.h"
#include "async++.h"
#include <iostream>
using namespace std;

TEST(aligned_array, isAligned) {
	using arr = typename async::detail::aligned_array<int>;
	arr arr1(2);
	cout << alignment_of<int>::value << endl;
	double x = 1.0;
	ASSERT_NEAR(x, 2.0, 1e-10);
}