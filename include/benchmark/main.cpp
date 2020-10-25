#include "benchmark.hpp"

#include <vector>
#include <unordered_map>

// 10
// Num iterations  : 128000
// Mean            : 39.6844
// Relative SD.    : 1.52863

std::vector<size_t> v(1000, 0);

auto sort_vector() {
	std::fill(v.begin(), v.end(), 0);	
	std::sort(v.begin(), v.end());
}

int main() {
	// auto b1 = benchmark(sort_vector);

	// [████████████████████] 100% 10001/10000 | Best Estimate: μ = 526ns ± 1.88%, N = 64000
	// auto b2 = benchmark([]() {
	// 	std::unordered_map<std::string, int> map;
	// 	map["a"] = 1;
	// 	map["b"] = 2;
	// 	map["c"] = 3;
	// 	map["a"] = 0;
	// });

	// auto b3 = benchmark([] {
	// 	return std::to_string(3.14);
	// });

	auto b4 = benchmark([] {
		volatile int i;
		std::vector<std::string> result;
		std::string foo;
		for (i = 0; i < 100; i++) {
			foo = std::string{"Hello World "} + std::to_string(i);
			result.push_back(foo);
		}
		return result;
	});
}