#include <array>
#include <charconv>
#include <string>

#include <catch2/catch_test_macros.hpp>

constexpr const char overtake_event = 'O';
constexpr const char incident_event = 'I';
constexpr const char separator = ' ';
constexpr const std::size_t size = 20UL;
constexpr const std::size_t incident_position = size;

static int champion_rank(int champion, const std::string& events) {
	const auto champion_index = static_cast<std::size_t>(champion - 1); // Arrays start at 0.
	assert(champion_index < size);

	std::array<std::size_t, size> position_of_pilot{};
	for (std::size_t index = 0UL; index < size; ++index) { position_of_pilot[index] = index; }
	std::array<std::size_t, size> pilot_at_position = position_of_pilot;

	std::size_t input_index = 0UL;
	while (input_index < events.size()) {
		const std::size_t input_number_end = events.find(separator, input_index);
		assert(input_number_end - input_index <= 2UL);

		std::size_t event_pilot{};
		std::from_chars(&events[input_index], &events[input_number_end], event_pilot);
		--event_pilot; // Arrays start at 0.
		assert(event_pilot < size);

		const std::size_t event_pilot_position = position_of_pilot[event_pilot];
		assert(event_pilot_position != incident_position);

		const std::size_t event_index = input_number_end + 1UL;
		const char event = events[event_index];
		switch (event) {
		case overtake_event: {
			assert(event_pilot_position > 0UL);
			const std::size_t overtaken_pilot_position = event_pilot_position - 1UL;
			const std::size_t overtaken_pilot = pilot_at_position[overtaken_pilot_position];

			std::swap(pilot_at_position[overtaken_pilot_position], pilot_at_position[event_pilot_position]);
			--position_of_pilot[event_pilot];
			++position_of_pilot[overtaken_pilot];
			break;
		}

		case incident_event: {
			// Move all pilots after the event pilot one position forward.
			for (std::size_t position = event_pilot_position + 1UL; position < size; ++position) {
				const std::size_t pilot = pilot_at_position[position];
				// Stop processing after reaching pilots that have suffered incidents, which are at the end of the array.
				if (position_of_pilot[pilot] == incident_position) { break; }

				const std::size_t previous_position = position - 1UL;
				std::swap(pilot_at_position[previous_position], pilot_at_position[position]);
				--position_of_pilot[pilot];
			}
			position_of_pilot[event_pilot] = incident_position;

			break;
		}
		default: assert(false); break;
		}

		constexpr std::size_t two_spaces_and_event = 3UL;
		input_index = input_number_end + two_spaces_and_event;
	}

	const std::size_t position_of_champion = position_of_pilot[champion_index];
	return position_of_champion == incident_position ?
					 -1 :
					 (static_cast<int>(position_of_champion) + 1); // Arrays start at zero.
}

TEST_CASE("20230410") {
	REQUIRE(champion_rank(3, "") == 3);
	REQUIRE(champion_rank(1, "2 O") == 2);
	REQUIRE(champion_rank(2, "2 O") == 1);
	REQUIRE(champion_rank(20, "20 I") == -1);
	REQUIRE(champion_rank(12, "4 O 3 O") == 12);
	REQUIRE(champion_rank(10, "1 I 10 O 2 I") == 7);
	REQUIRE(champion_rank(17, "2 O 17 I") == -1);
	REQUIRE(champion_rank(2, "9 O 17 O 9 O 12 O 2 O 12 O 9 O 1 O 5 O 12 O 17 O 20 O 16 O 7 O 2 O 8 O 16 O 14 O 3 O 14 O "
													 "11 O 16 O 1 O 13 O 8 O 14 O 5 O 12 O 4 O") == 1);
}
