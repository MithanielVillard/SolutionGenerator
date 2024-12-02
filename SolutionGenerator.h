#pragma once
#include <string>
#include <vector>

class SolutionGenerator
{
public:
    SolutionGenerator() = default;
    ~SolutionGenerator() = default;

    static void PrintHelpMessage();
    
    void CreateProject(const std::string& reposName, const std::string& solutionName, const std::string& projectName, bool pch = false, bool vcpkg = false);
    void MakeProject(const std::string& reposName);
    void AddVcpkgPort(const std::string& reposName, const std::string& port);

private:
    void GenerateSolFile(const std::string& reposName, const std::string& solutionName, const std::string& projectName);
    void GeneratePrjFile(const std::string& reposName, const std::string& solutionName);
    void AddProjectToPrj(const std::string& reposName, const std::string& projectName, bool vcpkg, bool pch);
    void GenerateSolution(const std::string& reposName, const std::vector<std::string>& projectsName, const std::vector<long>& projectsGuid);
    void GenerateVcxprojFile(const std::string& reposName, const std::string& projectName, long projectGuid, const std::string& pch, bool vcpkg);

    void PopulateVcxprojFile(const std::string& reposName, const std::string& projectName);

    void CreatePChFile(const std::string& reposName, const std::string& projectNamea);
    void CreateVcpkgFile(const std::string& projectName);

    std::string FindPrjFile(const std::string& reposName);

    void GetAllDirFiles(const std::string& dir, std::ofstream& vcxproj);

private:
    const std::vector<std::string> m_headerFilesExt = {".h", ".hpp", ".tpp", ".inl"};
    const std::vector<std::string> m_sourceFilesExt = {".c", ".cpp", ".c++"};
    const std::vector<std::string> m_otherFilesExt = {".jpeg" ,".jpg" ,".png", ".svg", ".ico", ".rc"};
};
