#include "benchmark.hpp"

#include <array>
#include <vector>
#include <unordered_map>

int main() {

	{
		// Array sort
		std::array<int, 5> a{0, 0, 0, 0, 0};
		benchmark("Array sort", [a] {
			auto arr = a;
			std::sort(arr.begin(), arr.end());
		});
	}

	// {
	// 	// ✔ Vector sort μ = 102ns ± 6.4%, N = 64000
	//   auto b = benchmark("Vector sort", [] {
	// 		std::vector<size_t> v(10, 0);
	//     std::sort(v.begin(), v.end());
	//   });
	// }

	// {
	// 	// [████████████████████] 100% 10001/10000 | Best Estimate: μ = 526ns ± 1.88%, N = 64000
	// 	benchmark("Unordered map insert", []() {
	// 		std::unordered_map<std::string, int> map;
	// 		map["a"] = 1;
	// 		map["b"] = 2;
	// 		map["c"] = 3;
	// 		map["a"] = 0;
	// 	});
	// }

	// {
	// 	// 5000/5000 μ = 168ns ± 2.79%, N = 64000
	// 	auto b = benchmark("to_string(double)", [] {
	// 		return std::to_string(3.14);
	// 	});
	// }

	// {
	// 	// 1000/1000 μ = 3.23us ± 15.1%, N = 32000
	// 	auto b = benchmark("Vector push_back string", [] {
	// 		volatile int i;
	// 		std::vector<std::string> result;
	// 		std::string foo;
	// 		for (i = 0; i < 100; i++) {
	// 			foo = std::string{"Hello World "} + std::to_string(i);
	// 			result.push_back(foo);
	// 		}
	// 		return result;
	// 	});
	// }

	// {
	// 	// 1038/1000 μ = 1.04us ± 1.21%, N = 32000
	// 	auto b = benchmark("Sum of vector of numbers", [] {
	// 		std::vector<int> numbers(1000, 2);
	// 		std::sort(numbers.begin(), numbers.end());
	// 		return std::accumulate(numbers.begin(), numbers.end(), 0);
	// 	});
	// }
}