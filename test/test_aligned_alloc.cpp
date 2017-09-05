#include "gtest/gtest.h"
//#include "aligned_alloc.h"
#include <iostream>
using namespace std;

TEST(aligned_array, isAligned) {
//	async::detail::aligned_array<int> arr1(2);
//	auto align = alignment_of<int>::value;
//	cout << align << endl;
	double x = 1.0;
	ASSERT_NEAR(x, 1.0, 1e-10);
}