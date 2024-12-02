#pragma once
#include <string>
#include <vector>

class SolutionGenerator
{
public:
    SolutionGenerator() = default;
    ~SolutionGenerator() = default;

    static void PrintHelpMessage();
    
    void CreateRepos(const std::string& reposName, const std::string& solutionName, const std::string& projectName,
        bool pch = false, bool vcpkg = false, bool window = false, bool lib = false);
    void MakeProject(const std::string& reposName, bool openExplorer = true);
    void AddVcpkgPort(const std::string& reposName, const std::string& projectName, const std::string& port);
    void CreateProject(const std::string& reposName, const std::string& projectName,
        bool pch = false, bool vcpkg = false, bool window = false, bool lib = false);

private:
    void GeneratePrjFile(const std::string& reposName, const std::string& solutionName);
    void GenerateSolution(const std::string& reposName, const std::vector<std::string>& projectsName, const std::vector<long>& projectsGuid);
    void GenerateVcxprojFile(const std::string& reposName, const std::string& projectName, long projectGuid, const std::string& pch, bool vcpkg);

    void PopulateVcxprojFile(const std::string& reposName, const std::string& projectName);

    void CreatePCHFile(const std::string& reposName, const std::string& projectNamea);
    void CreateVcpkgFile(const std::string& reposName, const std::string& projectName);

    std::string FindPrjFile(const std::string& reposName);

    void GetAllDirFiles(const std::string& dir, std::ofstream& vcxproj);

private:
    const std::vector<std::string> m_headerFilesExt = {".h", ".hpp", ".tpp", ".inl"};
    const std::vector<std::string> m_sourceFilesExt = {".c", ".cpp", ".c++"};
    const std::vector<std::string> m_otherFilesExt = {".jpeg" ,".jpg" ,".png", ".svg", ".ico", ".rc"};
};
