#include <vector>

#include <catch2/catch_test_macros.hpp>

// https://www.codewars.com/kata/54521e9ec8e60bc4de000d6c/train/cpp
static int maxSequence(const std::vector<int>& arr) {
	int max_sequence = 0;

	auto window_begin = arr.cbegin();

	const auto end = arr.cend();
	while (window_begin != end) {
		// Slide the window to cover as many positive solutions as possible.
		int current_sequence = *window_begin;
		auto window_end = window_begin + 1U;
		if (current_sequence > max_sequence) { max_sequence = current_sequence; }

		while (window_end != end && current_sequence >= 0) {
			current_sequence += *window_end;
			++window_end;

			if (current_sequence > max_sequence) { max_sequence = current_sequence; }
		}

		window_begin = window_end;
		// Slide the beginning of the window to the next positive value.
		while (window_begin != end && *window_begin <= 0) { ++window_begin; }
	}

	return max_sequence;
}

TEST_CASE("20230604") {
	REQUIRE(maxSequence({}) == 0);
	REQUIRE(maxSequence({2}) == 2);
	REQUIRE(maxSequence({-1, -890, -1}) == 0);
	REQUIRE(maxSequence({-1, 1, -1}) == 1);
	REQUIRE(maxSequence({-1, 1, -1, 4, -1}) == 4);
	REQUIRE(maxSequence({-2, 1, -3, 4, -1, 2, 1, -5, 4}) == 6);
	REQUIRE(maxSequence({2, -8, 3, -2, 4, -10}) == 5);
}
