#ifndef SETTINGS_H
#define SETTINGS_H

#define VERSION "beta 1.16.0 re"

#define HELP_MESSAGE \
    "\nLe Solution Generator est un outil permettant de generer des solutions Visual Studio.\n" \
    "Utilisation : SolutionGenerator.exe [Options]\n"\
    "\n"\
    "Options\n"\
    "+----------------------------------------------------------------------------------------------------------------+\n"\
    "|                                                                                                                |\n"\
    "|  -create <Nom_du_repos> <Nom_de_la_solution> {Nom_du_projet} {-pch} {-vcpkg}                                   |\n"\
    "|                                                                                                                |\n"\
    "|          Creer un projet avec le Solution Generator                                                            |\n"\
    "|          <Nom_du_repos> Le nom du repos GitHub dans lequel la solution va s'installer                          |\n"\
    "|          <Nom_de_la_solution> Le nom de la solution Visual Studio qui va etre cree                             |\n"\
    "|          {Nom_du_projet} Le nom du premier projet qui va etre cree (optionnel), par defaut <Nom_de_la_solution>|\n"\
    "|          {-pch} Active les header precompiles (pch) pour le projet (optionnel)                                 |\n"\
    "|          {-vcpkg} Active le gestionnaire de dependances vcpkg pour le projet (optionnel)                       |\n"\
    "|                                                                                                                |\n"\
    "|  -make <dossier_racine> {-clean}                                                                               |\n"\
    "|                                                                                                                |\n"\
    "|          <dossier_racine> Generer la solution a partir du dossier racine                                       |\n"\
    "|          {-clean} Retire les fichiers existants (optionnel)                                                    |\n"\
    "|                                                                                                                |\n"\
    "|  -add_port <Dependence> <Nom_du_repos>                                                                         |\n"\
    "|                                                                                                                |\n"\
    "|                                                                                                                |\n"\
    "|                                                                                                                |\n"\
    "|  -help   Montre ce message d'aide                                                                              |\n"\
    "|                                                                                                                |\n"\
    "+----------------------------------------------------------------------------------------------------------------+\n"

#define PCH_H \
    "#pragma once\n"

#define PCH_CPP \
    "#include \"pch.h\"\n"

#define VCPKG_JSON \
    "{\n"\
    "  \"dependencies\": [\n"\
    "  ]\n"\
    "}"

#define PRJ_FILE \
    "{\n"\
    "    \"solution_name\":\"\" \n"\
    "}"

#define PROJECT_JSON \
    "{\n"\
    R"(    "vcpkg": "false",)" \
    R"(    "pch": "false",)" \
    R"(    "window": "false",)" \
    R"(    "lib": "false")" \
    "}\n"\
    

    

#endif
