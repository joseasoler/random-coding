#include <vector>

#include <catch2/catch_test_macros.hpp>

using money_t = unsigned int;
using coin_t = unsigned int;
using count_t = unsigned long long;
using coin_vector = std::vector<coin_t>;
using coin_iterator = coin_vector::const_iterator;

static count_t count_change(money_t money, coin_iterator begin, coin_iterator end) {
	count_t current_count = 0U;
	for (auto coin_it = begin; coin_it != end; ++coin_it) {
		const coin_t coin = *coin_it;
		if (coin > money) {
			// The coins are sorted. When a coin larger than the current money is reached, stop the loop.
			break;
		} else if (coin == money) {
			// This coin is an end case.
			++current_count;
		} else {
			current_count += count_change(money - coin, coin_it, end);
		}
	}

	return current_count;
}

static count_t countChange(money_t money, const coin_vector& coins) {
	coin_vector sorted_coins(coins);
	std::sort(sorted_coins.begin(), sorted_coins.end());
	return count_change(money, sorted_coins.cbegin(), sorted_coins.cend());
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
