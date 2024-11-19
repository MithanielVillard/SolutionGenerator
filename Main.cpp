#include <iostream>
#include "Settings.h"
#include "SolutionGenerator.h"
#include "Utils.h"

int main(int argc, char* argv[])
{
    std::cout << "Solution Generator Version " << VERSION << " By ";
    Utils::CoutColored("Mird\n", Success);

    if (argc < 2)
    {
        Utils::CoutColored("Nombre d'argument incorect ! Utilise -help pour avoir de l'aide.\n", Error);
        return 1;
    }

    if (argv[1] == std::string("-help"))
    {
        SolutionGenerator::PrintHelpMessage();
        return 0;
    }

    if (argv[1] == std::string("-create"))
    {
        if (argc < 4)
        {
            Utils::CoutColored("Nombre d'argument incorect ! Utilise -help pour avoir de l'aide.\n", Error);
            return 1;
        }

        SolutionGenerator sg;
        if (argc == 4)
        {
            sg.CreateProject(argv[2], argv[3], argv[3]);
            return 0;
        }

        bool pch = argv[4] == std::string("-pch");
        bool vcpkg = argv[4] == std::string("-vcpkg");
        const std::string solutionName = argv[3];
        const std::string projectName = pch || vcpkg ? solutionName : argv[4];
        
        for (int i = 5; i < argc; i++)
        {
            pch |= argv[i] == std::string("-pch");
            vcpkg |= argv[i] == std::string("-vcpkg");
        }
        sg.CreateProject(argv[2], solutionName, projectName, pch, vcpkg);
        return 0;
    }

    if (argv[1] == std::string("-add_port"))
    {
        if (argc < 4)
        {
            Utils::CoutColored("Nombre d'argument incorect ! Utilise -help pour avoir de l'aide.\n", Error);
            return 1;
        }
        SolutionGenerator sg;
        sg.AddVcpkgPort(argv[3], argv[2]);
    }

    if (argv[1] == std::string("-make"))
    {
        if (argc < 3)
        {
            Utils::CoutColored("Nombre d'argument incorect ! Utilise -help pour avoir de l'aide.\n", Error);
            return 1;
        }
        SolutionGenerator sg;
        sg.MakeProject(argv[2]);
    }
    
    return 0;
}
