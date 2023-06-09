#include <memory>
#include <utility>

#include <catch2/catch_test_macros.hpp>

struct node {
	std::shared_ptr<node> next{};
	int v{};
};

template<typename Container> node to_list(const Container& container) {
	node list;
	node* node_current = &list;
	if (!container.empty()) { list.v = container.front(); }

	for (auto it_current = (container.cbegin() + 1); it_current != container.cend(); ++it_current) {
		node_current->next = std::make_unique<node>();
		node_current->next->v = *it_current;
		node_current = node_current->next.get();
	}

	return list;
}

static std::pair<const node*, std::size_t> size_and_last(const node* n) noexcept {
	std::size_t size{0U};

	while (n != nullptr) {
		++size;
		n = n->next.get();
	}

	return {n, size};
}

static const node* intersect(const node* lhs, const node* rhs) noexcept {
	// Error checking.
	if (lhs == nullptr || rhs == nullptr) { return nullptr; }

	const auto [lh_end, lh_size] = size_and_last(lhs);
	const auto [rh_end, rh_size] = size_and_last(rhs);

	// Lists that do not share an end do not intersect.
	if (lh_end != rh_end) { return nullptr; }

	// Node for checking the largest list. Moves ahead.
	const node* ahead_node{nullptr};
	// Node for checking the smaller list. Starts from the beginning.
	const node* current_node{nullptr};
	// Difference in size between both lists.
	std::size_t difference{};

	if (lh_size > rh_size) {
		ahead_node = lhs;
		current_node = rhs;
		difference = lh_size - rh_size;
	} else {
		ahead_node = rhs;
		current_node = lhs;
		difference = rh_size - lh_size;
	}

	// After moving this node ahead, they will eventually match the first intersection point.
	while (difference > 0U) {
		ahead_node = ahead_node->next.get();
		--difference;
	}

	while (ahead_node != current_node) {
		ahead_node = ahead_node->next.get();
		current_node = current_node->next.get();
	}

	return current_node;
}

TEST_CASE("20230612") {
	const node list1 = to_list(std::vector{3, 1, 5, 9, 7, 2, 1});
	auto intersection = list1.next->next->next->next;
	const node list2 = to_list(std::vector{4, 6});
	list2.next->next = intersection;

	REQUIRE(intersect(&list1, &list2) == intersection.get());
	REQUIRE(intersect(&list2, &list1) == intersection.get());
}
