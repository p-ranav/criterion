#include <string>
#include <vector>

struct StringAlgorithms {
  static std::vector<std::string> split(const std::string &string, const std::string &delimiter);
};

std::vector<std::string> StringAlgorithms::split(const std::string &string,
                                                 const std::string &delimiter) {
  std::vector<std::string> result;

  if (string.empty() || delimiter.empty()) {
    return result;
  }

  auto delimiter_position = string.find(delimiter);
  if (delimiter_position != std::string::npos) {
    result.push_back(string.substr(0, delimiter_position));

    // extend result with sub-result
    auto subresult = split(string.substr(delimiter_position + delimiter.size()), delimiter);
    result.reserve(result.size() + std::distance(subresult.begin(), subresult.end()));
    result.insert(result.end(), subresult.begin(), subresult.end());
  } else {
    result.push_back(string);
  }
  return result;
}