#include "getPath.h"

#include <filesystem>
#include <format>
#include <string>

auto getPath(const char* path) -> const char*
{
    std::filesystem::path currentPath = std::filesystem::current_path();

    return std::format("{}/{}", currentPath.string(), path).c_str();
}