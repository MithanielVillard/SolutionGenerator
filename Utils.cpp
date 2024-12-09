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

std::string Utils::GenerateGUID()
{
    GUID guidReference;
    HRESULT hCreateGuid = CoCreateGuid(&guidReference);
    if (FAILED(hCreateGuid))
    {
        CoutColored("Erreur lors de la generation de GUID du projet !", Error);
        return 0;
    }

    wchar_t wszGuid[40];
    StringFromGUID2(guidReference, wszGuid, 40);

    char szGuid[40];
    size_t convertedChars = 0;
    wcstombs_s(&convertedChars, szGuid, sizeof(szGuid), wszGuid, _TRUNCATE);
    
    return {szGuid};
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
