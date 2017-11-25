#include "gtest/gtest.h"
#include "asyncxx.h"
#include <iostream>

using T1 = typename asyncxx::detail::aligned_array<int>;
using T2 = typename asyncxx::detail::aligned_array<int, 32>;
constexpr size_t SIZE = 6;

TEST(aligned_array, isAligned) {
	T1 arr(2);
	ASSERT_EQ(((long)(arr.get()) & (std::alignment_of<int>::value-1)), 0);

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
#ifndef _WIN32
	ASSERT_EQ(arr3.get(), nullptr);
#endif
}

TEST(aligned_array, testConstructors) {
	T1 arr(SIZE);
	ASSERT_EQ(arr.size(), SIZE);
	ASSERT_TRUE(arr, true);
	auto address = arr.get();
	for (int i = 0; i < SIZE; ++i) arr[i] = i;

	T1 moveArr(std::move(arr));
	ASSERT_EQ(moveArr.size(), SIZE);
	ASSERT_TRUE(moveArr, true);
	for (int i = 0; i < SIZE; ++i) ASSERT_EQ(moveArr[i], i);
	auto moveAddress = moveArr.get();
	ASSERT_EQ(address, moveAddress);

	T1 moveAssignArr = std::move(moveArr);
	ASSERT_EQ(moveAssignArr.size(), SIZE);
	ASSERT_TRUE(moveAssignArr, true);
	for (int i = 0; i < SIZE; ++i) ASSERT_EQ(moveAssignArr[i], i);
	auto moveAssignAddress = moveAssignArr.get();
	ASSERT_EQ(address, moveAssignAddress);
}
