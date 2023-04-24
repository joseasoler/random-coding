#include <algorithm>
#include <array>
#include <cassert>
#include <memory>
#include <string_view>
#include <vector>

#include <catch2/catch_test_macros.hpp>

// Node sets are identified by an index.
using node_set_index = std::size_t;

// A node set contains between 0 and 9 nodes. It contains all the nodes for the "next digit".
using node_set = std::array<node_set_index, 10U>;

// The graph_data is a collection of node sets, indexed by their position in a vector.
using graph_data = std::vector<node_set>;

// Phone numbers are stored as string views.
using phone_number = std::string_view;

static bool valid_node_index(node_set_index index) {
	// The 0 index always corresponds to the first digit. It is impossible for
	// later digits to go back to a previous one.
	constexpr node_set_index invalid_node = 0UL;
	return index != invalid_node;
}

static void process_number(graph_data& graph, phone_number number) {
	node_set_index node_set_index = 0U;

	for (char digit_char : number) {
		const auto digit = static_cast<std::size_t>(digit_char - '0');

		assert(node_set_index <= graph.size());

		if (graph.size() == node_set_index) {
			graph.emplace_back();
			// By default, node sets are initialized to invalid node indexes.
			assert(!valid_node_index(graph[graph.size() - 1U][0U]));
		}

		// Insert a new node set if needed.
		if (!valid_node_index(graph[node_set_index][digit])) {
			const auto previous_size = graph.size();
			graph[node_set_index][digit] = previous_size;
		}

		const auto next_node_set_index = graph[node_set_index][digit];
		// Next node set has been updated correctly.
		assert(valid_node_index(next_node_set_index));

		// Since node sets are stored in a vector as they are found, they should never jump back to a previous one.
		assert(next_node_set_index > node_set_index);

		// Update values for the next iteration.
		node_set_index = next_node_set_index;
	}
}

// https://www.codewars.com/kata/582b59f45ad9526ae6000249/train/python
static std::size_t phone_numbers(const std::vector<std::string_view>& numbers) {
	graph_data graph;

	for (const auto& number : numbers) { process_number(graph, number); }

	std::size_t result = 0U;
	for (const auto& nodes : graph) {
		const auto valid_nodes = std::count_if(nodes.cbegin(), nodes.cend(), valid_node_index);
		assert(valid_nodes > 0);
		result += static_cast<std::size_t>(valid_nodes);
	}

	return result;
}

TEST_CASE("20230424") {
	REQUIRE(phone_numbers({"0", "1"}) == 2U);
	REQUIRE(phone_numbers({"01", "02", "03"}) == 4U);
	REQUIRE(phone_numbers({"012", "0123", "01234"}) == 5U);
	REQUIRE(phone_numbers({"111", "112", "113", "121", "122", "123"}) == 9U);
	REQUIRE(phone_numbers({"0123456789", "0123987654", "0123987456", "2365498756", "2365498765"}) == 31U);
	REQUIRE(phone_numbers({"0136", "0137", "0138", "0139", "0146", "0147", "0148", "0149", "0156", "0157", "0158", "0159",
						"0236", "0237", "0238", "0239", "0246", "0247", "0248", "0249", "0256", "0257", "0258", "0259"}) == 33);
	REQUIRE(phone_numbers(
						{"23307055", "23707015", "23707055", "23707955", "23717055", "23807055", "83707055", "93707055"}) == 46U);
	REQUIRE(phone_numbers({"10548970", "10946970", "10948770", "10948910", "10948960", "10948970", "10948976", "10948979",
						"40948970"}) == 35U);
	REQUIRE(phone_numbers({"476649781929", "476646281929", "476649284929", "476649251929", "476649281429", "476649981929",
						"476649285929", "476649281909", "476649681929"}) == 53U);
	REQUIRE(phone_numbers({"3", "99635", "454381", "45812", "4039173", "92", "777", "341944727", "5358", "882", "68",
						"684671", "8385687"}) == 52U);
}
