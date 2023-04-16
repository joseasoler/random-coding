#include <algorithm>
#include <cassert>
#include <limits>
#include <random>
#include <vector>

#include <catch2/catch_test_macros.hpp>

using vector = std::vector<int>;

static void mergesort(vector::iterator begin, vector::iterator end) {
	assert(begin < end);
	const auto size = std::distance(begin, end);
	assert(size > 0U);
	if (size == 1UL) { return; }
	if (size == 2UL && (*begin > *(begin + 1UL))) {
		std::iter_swap(begin, begin + 1UL);
		return;
	}
	const auto half_size = size / 2U;
	assert(size - half_size > 0U);
	const auto middle = begin + half_size;
	mergesort(begin, middle);
	mergesort(middle, end);

	// Extra space for sorting.
	const vector copy(begin, end);
	const auto copy_begin = copy.cbegin();
	const auto copy_middle = copy_begin + half_size;
	const auto copy_end = copy.cend();

	auto output = begin;
	auto first_half = copy_begin;
	auto second_half = copy_middle;

	while (output != end) {
		if (second_half == copy_end || (*first_half <= *second_half && first_half < copy_middle)) {
			*output = *first_half;
			++first_half;
		} else {
			*output = *second_half;
			++second_half;
		}
		assert(first_half <= second_half);
		++output;
	}
	assert(first_half == copy_middle);
	assert(second_half == copy_end);
}

TEST_CASE("20230402") {
	constexpr std::size_t size = 10UL;

	std::mt19937 rng{}; // NOLINT
	std::uniform_int_distribution<int> dist(std::numeric_limits<int>::min(), std::numeric_limits<int>::max());

	for (std::size_t iteration = 0UL; iteration < 1000UL; ++iteration) {
		vector reference_vector(size);
		std::generate(reference_vector.begin(), reference_vector.end(), [&dist, &rng]() { return dist(rng); });
		vector tested_vector(reference_vector);

		std::sort(reference_vector.begin(), reference_vector.end());
		mergesort(tested_vector.begin(), tested_vector.end());
		REQUIRE(reference_vector == tested_vector);
	}
}
