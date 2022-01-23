#pragma once
#include <fstream>
#include <string>
#include <vector>

#if __has_include(<experimental/filesystem>)
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#elif __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#elif __has_include(<boost/filesystem.hpp>)
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
#endif

namespace fips {

// Utility for loading files
// @param file_name location
// @returns vector of type T containing binary data from the file
// Caller ensures if the file's present
template <typename T> std::vector<T> LoadFile(const std::string &file_name) {
  std::ifstream in(file_name.c_str(), std::ios::binary);
  std::vector<char> raw_data((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
  return std::vector<T>(reinterpret_cast<T *>(raw_data.data()),
                        reinterpret_cast<T *>(raw_data.data()) + raw_data.size());
}
} // namespace fips
