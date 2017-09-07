#include "gtest/gtest.h"
#include "async++.h"
#include <iostream>
using namespace std;

using T1 = typename async::detail::aligned_array<int>;
using T2 = typename async::detail::aligned_array<int, 32>;

TEST(aligned_array, isAligned) {
	T1 arr(2);
	ASSERT_EQ(((long)(arr.get()) & (alignment_of<int>::value-1)), 0);

	T2 arr1(2);
	ASSERT_EQ((((long)(arr1.get())) & 31), 0);
}

TEST(aligned_array, toBool) {
	T1 arr(nullptr);
	ASSERT_FALSE(arr);
}

TEST(aligned_array, emptyArray) {
	T1 arr1(nullptr);
	ASSERT_EQ(arr1.size(), 0);
	ASSERT_EQ(arr1.get(), nullptr);

	T1 arr2;
	ASSERT_EQ(arr2.size(), 0);
	ASSERT_EQ(arr2.get(), nullptr);

	T1 arr3((size_t)0);
	ASSERT_EQ(arr3.size(), 0);
	ASSERT_EQ(arr3.get(), nullptr);
}