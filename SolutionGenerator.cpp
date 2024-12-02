#include "SolutionGenerator.h"
#include "Settings.h"
#include "Utils.h"

#include <iostream>
#include <ostream>
#include <filesystem>
#include <fstream>
#include <windows.h>

#include "Libs/Json.hpp"

namespace fs = std::filesystem;

void SolutionGenerator::PrintHelpMessage()
{
    std::cout << HELP_MESSAGE << "\n";
}

void SolutionGenerator::CreateProject(const std::string& reposName, const std::string& solutionName, const std::string& projectName, bool pch, bool vcpkg)
{
    if(fs::exists(reposName))
    {
        Utils::CoutColored("Un dossier portant ce nom existe deja !\n", Error);
        return;
    }

    if (fs::create_directory(reposName))
    {
        fs::create_directories(reposName + "/config");
        fs::create_directories(reposName + "/ide");
        if (fs::create_directories(reposName + "/src"))
        {
            fs::create_directories(reposName + "/src/"+projectName);
        }
        fs::create_directories(reposName + "/res");
        std::ofstream gitignore(reposName + "/gitignore");
        gitignore << "ide";
        gitignore.close();

        if (pch) CreatePChFile(reposName, projectName);
        if (vcpkg) CreateVcpkgFile(reposName);

        std::cout << "Arborescence cree \n";
        
        //GenerateSolFile(reposName, solutionName, projectName);
        GeneratePrjFile(reposName, solutionName);
        AddProjectToPrj(reposName, projectName, vcpkg, pch);
    }
    MakeProject(reposName);
    Utils::CoutColored("Projet creer avec succes !\n", Success);
}

void SolutionGenerator::CreatePChFile(const std::string& reposName, const std::string& projectName)
{
    std::ofstream pchH(reposName + "/src/"+projectName+"/pch.h");
    pchH << PCH_H;
    pchH.close();

    std::ofstream pchCpp(reposName + "/src/"+projectName+"/pch.cpp");
    pchCpp << PCH_CPP;
    pchCpp.close();
}

void SolutionGenerator::CreateVcpkgFile(const std::string& reposName)
{
    std::ofstream vcpkgFile(reposName + "/config/vcpkg.json");
    vcpkgFile << VCPKG_JSON; // todo A VOIR
    vcpkgFile.close();

    std::ofstream configFile(reposName + "/config/vcpkg-configuration.json");
    nlohmann::json config = {
        {"default-registry", {
            {"kind", "git"},
            {"baseline", "5a3e638f449206ac10f82f2171a21333004d7306"},
            {"repository", "https://github.com/microsoft/vcpkg"}
        }},
        {"registries", nlohmann::json::array({
            {
                {"kind", "artifact"},
                {"location", "https://github.com/microsoft/vcpkg-ce-catalog/archive/refs/heads/main.zip"},
                {"name", "microsoft"}
            }
        })}
    };
    configFile << std::setw(4) << config;
    configFile.close();
}

void SolutionGenerator::GenerateSolution(const std::string& reposName, const std::vector<std::string>& projectsName, const std::vector<long>& projectsGuid)
{
    std::ifstream solFile;
    std::string solutionName = FindPrjFile(reposName);
    if (solutionName.empty()) return;
    solFile.open(solutionName);
    nlohmann::json solJson = nlohmann::json::parse(solFile);

    std::ofstream file(reposName+"/ide/vs/"+std::string(solJson["solution_name"])+".sln");
    file << "Microsoft Visual Studio Solution File, Format Version 12.00\n";
    file << "# Visual Studio Version 17\n";
    file << "VisualStudioVersion = 17.3.31903.123\n";
    file << "MinimumVisualStudioVersion = 10.0.40219.1\n";

    for (int i = 0; i < projectsName.size(); ++i)
        file << "Project(\"{" << projectsGuid[i] << "}\") = \"" << projectsName[i] << "\", \"" << projectsName[i] << "\\" << projectsName[i] << ".vcxproj\", \"{" << projectsGuid[i] << "}\"\n";
    file << "EndProject\n";


    file << "Global\n";
    file << "\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\n";
    file << "\t\tDebug|x64 = Debug|x64\n";
    file << "\t\tRelease|x64 = Release|x64\n";
    file << "\tEndGlobalSection\n";
    file << "\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\n";
    for (const long& projectGuid : projectsGuid)
    {
        file << "\t\t{" << projectGuid << "}.Debug|x64.ActiveCfg = Debug|x64\n";
        file << "\t\t{" << projectGuid << "}.Debug|x64.Build.0 = Debug|x64\n";
        file << "\t\t{" << projectGuid << "}.Release|x64.ActiveCfg = Release|x64\n";
        file << "\t\t{" << projectGuid << "}.Release|x64.Build.0 = Release|x64\n";
    }
    file << "\tEndGlobalSection\n";
    file << "EndGlobal\n";
    file.close();
}

std::string SolutionGenerator::FindPrjFile(const std::string& reposName)
{
    for(auto& entry : fs::directory_iterator(reposName+"/config"))
    {
        if (entry.path().extension() == ".prj")
        {
            return entry.path().string();
        }
    }
    Utils::CoutColored("Pas de fichier .prj trouve", Error);
    return std::string("");
}

void SolutionGenerator::GenerateVcxprojFile(const std::string& reposName, const std::string& projectName, long projectGuid, const std::string& pch, const bool vcpkg)
{
    std::ofstream vcxprojFile(reposName+"/ide/vs/"+projectName+"/"+projectName+".vcxproj");
    
   vcxprojFile << R"(<?xml version="1.0" encoding="utf-8"?>)" << "\n";
    vcxprojFile << R"(<Project xmlns="http://schemas.microsoft.com/developer/msbuild/2003" ToolsVersion="4.0">)" << "\n";

    vcxprojFile << R"(  <ItemGroup Label="ProjectConfigurations">)" << "\n";
    vcxprojFile << R"(    <ProjectConfiguration Include="Debug|x64">)" << "\n";
    vcxprojFile << R"(      <Configuration>Debug</Configuration>)" << "\n";
    vcxprojFile << R"(      <Platform>x64</Platform>)" << "\n";
    vcxprojFile << R"(    </ProjectConfiguration>)" << "\n";
    vcxprojFile << R"(    <ProjectConfiguration Include="Release|x64">)" << "\n";
    vcxprojFile << R"(      <Configuration>Release</Configuration>)" << "\n";
    vcxprojFile << R"(      <Platform>x64</Platform>)" << "\n";
    vcxprojFile << R"(    </ProjectConfiguration>)" << "\n";
    vcxprojFile << R"(  </ItemGroup>)" << "\n";

    vcxprojFile << R"(  <PropertyGroup Label="Globals">)" << "\n";
    vcxprojFile <<  "     <ProjectGuid>{"+std::to_string(projectGuid)+"}</ProjectGuid>" << "\n";
    vcxprojFile <<  "     <RootNamespace>"+projectName+"</RootNamespace>" << "\n";
    vcxprojFile << R"(    <Keyword>Win32Proj</Keyword>)" << "\n";
    vcxprojFile << R"(  </PropertyGroup>)" << "\n";

    if (vcpkg)
    {
        vcxprojFile << R"(  <PropertyGroup Label="Vcpkg">)" << "\n";
        vcxprojFile << R"(    <VcpkgEnable>true</VcpkgEnable>)" << "\n";
        vcxprojFile << R"(    <VcpkgEnableManifest>true</VcpkgEnableManifest>)" << "\n";
        vcxprojFile << R"(    <VcpkgManifestRoot>..\..\..\config\</VcpkgManifestRoot>)" << "\n";
        vcxprojFile << R"(    <VcpkgInstalledDir>.\vcpkg_installed\$(VcpkgTriplet)\</VcpkgInstalledDir>)" << "\n";
        vcxprojFile << R"(  </PropertyGroup>)" << "\n";

        vcxprojFile << R"(  <PropertyGroup Label="Vcpkg" Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">)" << "\n";
        vcxprojFile << R"(    <VcpkgConfiguration>Debug</VcpkgConfiguration>)" << "\n";
        vcxprojFile << R"(  </PropertyGroup>)" << "\n";

        vcxprojFile << R"(  <PropertyGroup Label="Vcpkg" Condition="'$(Configuration)|$(Platform)'=='Release|x64'">)" << "\n";
        vcxprojFile << R"(    <VcpkgConfiguration>Release</VcpkgConfiguration>)" << "\n";
        vcxprojFile << R"(  </PropertyGroup>)" << "\n";
    }

    vcxprojFile << R"(  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.Default.props" />)" << "\n";

    vcxprojFile << R"(  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">)" << "\n";
    vcxprojFile << R"(    <ConfigurationType>Application</ConfigurationType>)" << "\n";
    vcxprojFile << R"(    <OutDir>$(ProjectDir)Build\$(Configuration)\</OutDir>)" << "\n";
    vcxprojFile << R"(    <IntDir>$(ProjectDir)Build\$(Configuration)\intermediate\</IntDir>)" << "\n";
    vcxprojFile << R"(    <PlatformToolset>v143</PlatformToolset>)" << "\n";
    vcxprojFile << R"(    <GenerateDebugInformation>true</GenerateDebugInformation>)" << "\n";
    vcxprojFile << R"(  </PropertyGroup>)" << "\n";

    vcxprojFile << R"(  <PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">)" << "\n";
    vcxprojFile << R"(    <ConfigurationType>Application</ConfigurationType>)" << "\n";
    vcxprojFile << R"(    <OutDir>$(ProjectDir)Build\$(Configuration)\</OutDir>)" << "\n";
    vcxprojFile << R"(    <IntDir>$(ProjectDir)Build\$(Configuration)\intermediate\</IntDir>)" << "\n";
    vcxprojFile << R"(    <PlatformToolset>v143</PlatformToolset>)" << "\n";
    vcxprojFile << R"(  </PropertyGroup>)" << "\n";

    vcxprojFile << R"(  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">)" << "\n";
    vcxprojFile << R"(    <ClCompile>)" << "\n";
    vcxprojFile << R"(      <WarningLevel>Level3</WarningLevel>)" << "\n";
    vcxprojFile << R"(      <SDLCheck>true</SDLCheck>)" << "\n";
    vcxprojFile << R"(      <PreprocessorDefinitions>_DEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>)" << "\n";
    vcxprojFile << R"(      <ConformanceMode>true</ConformanceMode>)" << "\n";
    vcxprojFile <<  "       <PrecompiledHeader>"+pch+"</PrecompiledHeader>" << "\n";
    vcxprojFile << R"(      <PrecompiledHeaderFile>pch.h</PrecompiledHeaderFile>)" << "\n";
    vcxprojFile << R"(      <LanguageStandard>stdcpp20</LanguageStandard>)" << "\n";
    vcxprojFile << R"(    </ClCompile>)" << "\n";
    vcxprojFile << R"(    <Link>)" << "\n";
    vcxprojFile << R"(      <SubSystem>Console</SubSystem>)" << "\n";
    vcxprojFile << R"(      <GenerateDebugInformation>true</GenerateDebugInformation>)" << "\n";
    vcxprojFile << R"(      <AdditionalDependencies>%(AdditionalDependencies)</AdditionalDependencies>)" << "\n";
    vcxprojFile << R"(    </Link>)" << "\n";
    vcxprojFile << R"(  </ItemDefinitionGroup>)" << "\n";

    vcxprojFile << R"(  <ItemDefinitionGroup Condition="'$(Configuration)|$(Platform)'=='Release|x64'">)" << "\n";
    vcxprojFile << R"(    <ClCompile>)" << "\n";
    vcxprojFile << R"(      <WarningLevel>Level3</WarningLevel>)" << "\n";
    vcxprojFile << R"(      <SDLCheck>true</SDLCheck>)" << "\n";
    vcxprojFile << R"(      <PreprocessorDefinitions>NDEBUG;_CONSOLE;%(PreprocessorDefinitions)</PreprocessorDefinitions>)" << "\n";
    vcxprojFile << R"(      <ConformanceMode>true</ConformanceMode>)" << "\n";
    vcxprojFile <<  "       <PrecompiledHeader>"+pch+"</PrecompiledHeader>" << "\n";
    vcxprojFile << R"(      <PrecompiledHeaderFile>pch.h</PrecompiledHeaderFile>)" << "\n";
    vcxprojFile << R"(      <LanguageStandard>stdcpp20</LanguageStandard>)" << "\n";
    vcxprojFile << R"(    </ClCompile>)" << "\n";
    vcxprojFile << R"(    <Link>)" << "\n";
    vcxprojFile << R"(      <SubSystem>Console</SubSystem>)" << "\n";
    vcxprojFile << R"(      <GenerateDebugInformation>false</GenerateDebugInformation>)" << "\n";
    vcxprojFile << R"(      <AdditionalDependencies>%(AdditionalDependencies)</AdditionalDependencies>)" << "\n";
    vcxprojFile << R"(    </Link>)" << "\n";
    vcxprojFile << R"(  </ItemDefinitionGroup>)" << "\n";

    vcxprojFile << R"(  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.props" />)" << "\n";

    if (pch == "Use")
    {
        vcxprojFile <<  "<ItemGroup>" << "\n";
        vcxprojFile << R"( <ClCompile Include="..\..\..\src\)"+projectName+R"(\pch.cpp">)" << "\n";
        vcxprojFile << R"(    <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Debug|x64'">Create</PrecompiledHeader>)" << "\n";
        vcxprojFile << R"(    <PrecompiledHeader Condition="'$(Configuration)|$(Platform)'=='Release|x64'">Create</PrecompiledHeader>)" << "\n";
        vcxprojFile <<  "  </ClCompile>" << "\n";
        vcxprojFile <<  "</ItemGroup>" << "\n";
    }

    vcxprojFile.close();
}

void SolutionGenerator::GenerateSolFile(const std::string& reposName, const std::string& solutionName, const std::string& projectName)
{
    nlohmann::json solJson = 
    {
        {"solution_name", solutionName},
        {"format_version", "12.00"},
        {"version", "16"},
        {"version_full", "16.0.30319.14"},
        {"minimum_version", "10.0.40219.1"},
        {"projects", projectName + ".prj"}
    };
    std::ofstream solFile(reposName+"/config/"+projectName+".sol");
    solFile << std::setw(4) << solJson;
    solFile.close();
    std::cout << "Fichier .sol genere \n";
}

void SolutionGenerator::GeneratePrjFile(const std::string& reposName, const std::string& solutionName)
{
    std::ofstream prjFile(reposName+"/config/"+solutionName+".prj");
    nlohmann::json prjJson = nlohmann::json::parse(PRJ_FILE);

    prjJson["solution_name"] = solutionName;
    prjFile << std::setw(4) << prjJson;
    
    prjFile.close();
    std::cout << "Fichier .prj genere \n";
}

void SolutionGenerator::AddProjectToPrj(const std::string& reposName, const std::string& projectName, bool vcpkg,
    bool pch)
{
    std::string prjPath = FindPrjFile(reposName);
    std::ifstream prjFileRead(prjPath);
    
    nlohmann::json prjJson = nlohmann::json::parse(prjFileRead);
    prjJson[projectName] =  nlohmann::json::parse(PROJECT_JSON);
    prjJson[projectName]["vcpkg"] = vcpkg ? "true" : "false";
    prjJson[projectName]["pch"] = pch ? "true" : "false";
    prjFileRead.close();
    
    std::ofstream prjFileWrite(prjPath);
    prjFileWrite << std::setw(4) << prjJson;
    prjFileWrite.close();
    
    Utils::CoutColored("Project " + projectName + " added\n", Success);
}

void SolutionGenerator::MakeProject(const std::string& reposName)
{
    fs::create_directories(reposName+"/ide/vs");

    std::vector<std::string> projectsName;
    std::vector<long> projectsGuid;

    std::ifstream prjFile(FindPrjFile(reposName));
    nlohmann::json prjJson = nlohmann::json::parse(prjFile);

    for(auto& element : fs::directory_iterator(reposName+"/src"))
    {
        std::string projectName = element.path().filename().string();

        if (!prjJson.contains(projectName))
        {
            Utils::CoutColored("Dossier " + projectName + " detecte dans le repertoire src mais sans configuration dans le fichier .prj", Warning);
            continue;
        }

        fs::create_directories(reposName+"/ide/vs/"+=projectName);
        long projectGuid = Utils::GenerateGUID();
        
        GenerateVcxprojFile(reposName, projectName, projectGuid, prjJson[projectName]["pch"] == std::string("true") ? "Use" : "NotUsing", prjJson[projectName]["vcpkg"] == std::string("true"));
        PopulateVcxprojFile(reposName, projectName);

        projectsName.push_back(projectName);
        projectsGuid.push_back(projectGuid);

        Utils::CoutColored("Projet ", Success);
        Utils::CoutColored(projectName, Warning);
        Utils::CoutColored(" Genere\n", Success);
    }

    prjFile.close();
    GenerateSolution(reposName, projectsName, projectsGuid);
    Utils::CoutColored("Solution generee avec succes !\n", Success);
    ShellExecuteA(NULL, "open", fs::absolute(reposName+"/ide/vs").string().c_str(), NULL, NULL, SW_SHOWNORMAL);
}

void SolutionGenerator::PopulateVcxprojFile(const std::string& reposName, const std::string& projectName)
{
    std::string vcxprojPath = reposName+"/ide/vs/"+projectName+"/"+projectName+".vcxproj";
    
    std::ofstream vcxprojFile(vcxprojPath, std::ios_base::app);
    
    vcxprojFile <<  "  <ItemGroup>\n";
    GetAllDirFiles(reposName+"/src/"+projectName, vcxprojFile);
    vcxprojFile <<  "  </ItemGroup>\n";
    vcxprojFile <<R"(  <Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets" />)" << '\n';
    vcxprojFile <<  "</Project>";

    vcxprojFile.close();
}

void SolutionGenerator::GetAllDirFiles(const std::string& dir, std::ofstream& vcxproj)
{
    for (const auto& entry : fs::directory_iterator(dir))
    {
        if (entry.is_directory())
        {
            GetAllDirFiles(entry.path().string(), vcxproj);
            continue;
        }
        if (entry.path().filename().string() == "pch.cpp") continue;

        const std::string fileExt = entry.path().extension().string();
        if (std::find(m_headerFilesExt.begin(), m_headerFilesExt.end(), fileExt) != m_headerFilesExt.end())
        {
            vcxproj << R"(    <ClInclude Include=")"+Utils::GetSrcFilePath(entry.path()) << R"("/>)" << "\n";
            continue;
        }
        if (std::find(m_sourceFilesExt.begin(), m_sourceFilesExt.end(), fileExt) != m_sourceFilesExt.end())
        {
            vcxproj << R"(    <ClCompile Include=")"+Utils::GetSrcFilePath(entry.path()) << R"("/>)" << "\n";
            continue;
        }
        if (std::find(m_otherFilesExt.begin(), m_otherFilesExt.end(), fileExt) != m_otherFilesExt.end())
            vcxproj << R"(    <None Include=")"+Utils::GetSrcFilePath(entry.path()) << R"("/>)" << "\n";
    }
}

void SolutionGenerator::AddVcpkgPort(const std::string& reposName, const std::string& port)
{
    if (!fs::exists(reposName+"/config/vcpkg.json"))
    {
        Utils::CoutColored("vcpkg n'est pas active pour ce projet !", Error);
        return;
    }
    std::ifstream vcpkgFile(reposName+"/config/vcpkg.json");
    nlohmann::json vcpkgJson = nlohmann::json::parse(vcpkgFile);

    vcpkgJson["dependencies"].push_back(port);
    
    std::ofstream vcpkgFileStream(reposName+"/config/vcpkg.json");
    vcpkgFileStream << std::setw(4) << vcpkgJson;
    vcpkgFile.close();
    vcpkgFileStream.close();
    
    Utils::CoutColored("Dependence ", Success);
    Utils::CoutColored(port, Warning);
    Utils::CoutColored(" ajoutee avec succes ^^", Success);
}
