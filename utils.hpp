#include <array>
#include <random>

std::random_device device;
std::mt19937 generator(device());

namespace utils
{
	inline int32_t generate_random(int32_t start, int32_t stop)
	{
		std::uniform_int_distribution<std::mt19937::result_type> distribution(start, stop);

		return distribution(generator);
	}

	int64_t map(int64_t value, int64_t in_min, int64_t in_max, int64_t out_min, int64_t out_max)
	{
		return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	}
};
