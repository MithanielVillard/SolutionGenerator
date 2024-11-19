#include "Utils.h"
#include <iostream>
#include <windows.h> 

void Utils::CoutColored(const std::string& message, Colors color)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
    std::cout << message;
    SetConsoleTextAttribute(hConsole, Normal);
}

long Utils::GenerateGUID()
{
    GUID guidReference;
    HRESULT hCreateGuid = CoCreateGuid(&guidReference);
    if (FAILED(hCreateGuid))
    {
        CoutColored("Erreur lors de la generation de GUID du projet !", Error);
        return 0;
    }
    return guidReference.Data1;
}

std::string Utils::GetSrcFilePath(const std::filesystem::path& path)
{
    std::filesystem::path src("..\\..\\..\\");
    auto it = path.begin();
    ++it;

    for (; it != path.end(); ++it)
    {
        src /= *it;
    }
    return src.string();
}
