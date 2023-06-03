#include <vector>

#include <catch2/catch_test_macros.hpp>

using money_t = unsigned int;
using coin_t = unsigned int;
using count_t = unsigned long long;

// https://www.codewars.com/kata/541af676b589989aed0009e7/train/cpp
static count_t countChange(const money_t money, const std::vector<coin_t>& coins) {
	std::vector<count_t> solutions_by_count(money + 1U, 0U);
	// There is always one combination for getting no money, taking no coins.
	solutions_by_count[0U] = 1U;
	for (const coin_t current_coin : coins) {
		for (count_t current_count = current_coin; current_count < (money + 1U); ++current_count) {
			// If the current count minus the current coin has some solutions, then the current count will also have those
			// solutions (existing combinations + current coin).
			// Since solutions_by_count[0] is 1, this will add all amounts divisible by this coin as partial solutions.
			solutions_by_count[current_count] += solutions_by_count[current_count - current_coin];
		}
	}
	return solutions_by_count.back();
}

TEST_CASE("20230503") {
	REQUIRE(countChange(4, {1, 2}) == 3);
	REQUIRE(countChange(11, {5, 7}) == 0);
	REQUIRE(countChange(98, {3, 14, 8}) == 19);
	REQUIRE(countChange(199, {3, 5, 9, 15}) == 760);
	REQUIRE(countChange(300, {5, 10, 20, 50, 100, 200, 500}) == 1022);
	REQUIRE(countChange(301, {5, 10, 20, 50, 100, 200, 500}) == 0);
	REQUIRE(countChange(419, {2, 5, 10, 20, 50}) == 18515);
}
