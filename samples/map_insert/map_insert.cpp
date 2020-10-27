#include <benchmark/benchmark.hpp>
#include <map>
#include <unordered_map>
#include <vector>

BENCHMARK("Insert 1 item into map<int, int>")
{
  std::map<int, int> map;
  map.insert(std::make_pair(1, 2));
}

BENCHMARK("Insert 10 items into map<int, int>")
{
  std::map<int, int> map;
  for (auto i = 0; i < 10; i++) {
    map.insert(std::make_pair(i, i * 2));
  }
}

BENCHMARK("Insert 1000000 items into map<int, int>")
{
  std::unordered_map<int, int> map;
  for (auto i = 0; i < 1000000; i++) {
    map.insert(std::make_pair(i, i * 2));
  }
}

std::vector<std::string> keys{"a", "b", "c", "d", "e", "f", "g", "h", "i", "j"};
std::vector<std::string> values{"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"};

BENCHMARK("Insert 1 item into map<string, string>")
{
  std::map<std::string, std::string> map;
  map.insert(std::make_pair("a", "1"));
}

BENCHMARK("Insert 10 items into map<string, string>")
{
  std::map<std::string, std::string> map;
  for (size_t i = 0; i < keys.size(); i++) {
    map.insert(std::make_pair(keys[i], values[i]));
  }
}

BENCHMARK("Insert 1 item into unordered_map<int, int>")
{
  std::unordered_map<int, int> map;
  map.insert(std::make_pair(1, 2));
}

BENCHMARK("Insert 10 items into unordered_map<int, int>")
{
  std::unordered_map<int, int> map;
  for (auto i = 0; i < 10; i++) {
    map.insert(std::make_pair(i, i * 2));
  }
}

BENCHMARK("Insert 1000000 items into unordered_map<int, int>")
{
  std::unordered_map<int, int> map;
  for (auto i = 0; i < 1000000; i++) {
    map.insert(std::make_pair(i, i * 2));
  }
}

BENCHMARK("Insert 1 item into unordered_map<string, string>")
{
  std::unordered_map<std::string, std::string> map;
  map.insert(std::make_pair("a", "1"));
}

BENCHMARK("Insert 10 items into unordered_map<string, string>")
{
  std::unordered_map<std::string, std::string> map;
  for (size_t i = 0; i < keys.size(); i++) {
    map.insert(std::make_pair(keys[i], values[i]));
  }
}