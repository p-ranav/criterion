#include <criterion/criterion.hpp>
#include <memory>
#include <string>

struct Song {
  std::string artist;
  std::string title;
  Song(const std::string& artist_, const std::string& title_) :
    artist{ artist_ }, title{ title_ } {}
};

BENCHMARK(ConstructSharedPtr, std::function<std::shared_ptr<Song>()>) 
{
  SETUP_BENCHMARK(
    auto test_function = GET_ARGUMENT(0);
  )

  // Code to be benchmarked
  auto song_ptr = test_function();
}

// Functions to be tested
auto Create_With_New() { return std::shared_ptr<Song>(new Song("Black Sabbath", "Paranoid")); }
auto Create_With_MakeShared() { return std::make_shared<Song>("Black Sabbath", "Paranoid"); }

INVOKE_BENCHMARK_FOR_EACH(ConstructSharedPtr, 
  ("/new", Create_With_New),
  ("/make_shared", Create_With_MakeShared)
)