#pragma once
#include <filesystem>
#include <string>

enum Colors : char
{
    Success = 10, Error = 12, Warning = 14, Normal = 15
};

class Utils
{
public:
    static void CoutColored(const std::string& message, Colors color);
    static std::string GenerateGUID();
    static std::string GetSrcFilePath(const std::filesystem::path& path);
};
